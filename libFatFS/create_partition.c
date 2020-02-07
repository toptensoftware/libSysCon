/*----------------------------------------------------------------------------/
/  FatFs - Generic FAT Filesystem Module  R0.14                               /
/-----------------------------------------------------------------------------/
/
/ Copyright (C) 2019, ChaN, all right reserved.
/
/ FatFs module is an open source software. Redistribution and use of FatFs in
/ source and binary forms, with or without modification, are permitted provided
/ that the following condition is met:
/
/ 1. Redistributions of source code must retain the above copyright notice,
/    this condition and the following disclaimer.
/
/ This software is provided by the copyright holder and contributors "AS IS"
/ and any warranties related to this software are DISCLAIMED.
/ The copyright owner or contributors be NOT LIABLE for any damages caused
/ by use of this software.
/
/----------------------------------------------------------------------------*/


#include "ff.h"			/* Declarations of FatFs API */
#include "ffpriv.h"		/* Private declarations */

#if !FF_FS_READONLY && FF_USE_MKFS
/*-----------------------------------------------------------------------*/
/* Create an FAT/exFAT volume                                            */
/*-----------------------------------------------------------------------*/

/* Create partitions on the physical drive */

FRESULT create_partition (
	BYTE drv,			/* Physical drive number */
	const LBA_t plst[],	/* Partition list */
	UINT sys,			/* System ID (for only MBR, temp setting) and bit8:GPT */
	BYTE* buf			/* Working buffer for a sector */
)
{
	UINT i, cy;
	LBA_t sz_drv;
	DWORD sz_drv32, s_lba32, n_lba32;
	BYTE *pte, hd, n_hd, sc, n_sc;

	/* Get drive size */
	if (disk_ioctl(drv, GET_SECTOR_COUNT, &sz_drv) != RES_OK) return FR_DISK_ERR;

#if FF_LBA64
	if (sz_drv >= FF_MIN_GPT) {	/* Create partitions in GPT */
		WORD ss;
		UINT sz_pt, pi, si, ofs;
		DWORD bcc, rnd, align;
		QWORD s_lba64, n_lba64, sz_pool, s_bpt;
		static const BYTE gpt_mbr[16] = {0x00, 0x00, 0x02, 0x00, 0xEE, 0xFE, 0xFF, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};

#if FF_MAX_SS != FF_MIN_SS
		if (disk_ioctl(drv, GET_SECTOR_SIZE, &ss) != RES_OK) return FR_DISK_ERR;	/* Get sector size */
		if (ss > FF_MAX_SS || ss < FF_MIN_SS || (ss & (ss - 1))) return FR_DISK_ERR;
#else
		ss = FF_MAX_SS;
#endif
		rnd = GET_FATTIME();			/* Random seed */
		align = GPT_ALIGN / ss;			/* Partition alignment [sector] */
		sz_pt = GPT_ITEMS * SZ_GPTE / ss;	/* Size of PT [sector] */
		s_bpt = sz_drv - sz_pt - 1;		/* Backup PT start sector */
		s_lba64 = 2 + sz_pt;			/* First allocatable sector */
		sz_pool = s_bpt - s_lba64;		/* Size of allocatable area */
		bcc = 0xFFFFFFFF; n_lba64 = 1;
		pi = si = 0;	/* partition table index, size table index */
		do {
			if (pi * SZ_GPTE % ss == 0) mem_set(buf, 0, ss);	/* Clean the buffer if needed */
			if (n_lba64 != 0) {	/* Is the size table not termintated? */
				s_lba64 = (s_lba64 + align - 1) & ((QWORD)0 - align);	/* Align partition start */
				n_lba64 = plst[si++];	/* Get a partition size */
				if (n_lba64 <= 100) {	/* Is the size in percentage? */
					n_lba64 = sz_pool * n_lba64 / 100;
					n_lba64 = (n_lba64 + align - 1) & ((QWORD)0 - align);	/* Align partition end (only if in percentage) */
				}
				if (s_lba64 + n_lba64 > s_bpt) {	/* Clip at end of the pool */
					n_lba64 = (s_lba64 < s_bpt) ? s_bpt - s_lba64 : 0;
				}
			}
			if (n_lba64 != 0) {		/* Add a partition? */
				ofs = pi * SZ_GPTE % ss;
				mem_cpy(buf + ofs + GPTE_PtGuid, GUID_MS_Basic, 16);	/* Partition GUID (Microsoft Basic Data) */
				rnd = make_rand(rnd, buf + ofs + GPTE_UpGuid, 16);		/* Unique partition GUID */
				st_qword(buf + ofs + GPTE_FstLba, s_lba64);				/* Partition start LBA */
				st_qword(buf + ofs + GPTE_LstLba, s_lba64 + n_lba64 - 1);	/* Partition end LBA */
				s_lba64 += n_lba64;		/* Next partition LBA */
			}
			if ((pi + 1) * SZ_GPTE % ss == 0) {		/* Write the buffer if it is filled up */
				for (i = 0; i < ss; bcc = crc32(bcc, buf[i++])) ;	/* Calculate table check sum */
				if (disk_write(drv, buf, 2 + pi * SZ_GPTE / ss, 1) != RES_OK) return FR_DISK_ERR;		/* Primary table */
				if (disk_write(drv, buf, s_bpt + pi * SZ_GPTE / ss, 1) != RES_OK) return FR_DISK_ERR;	/* Secondary table */
			}
		} while (++pi < GPT_ITEMS);

		/* Create primary GPT header */
		mem_set(buf, 0, ss);
		mem_cpy(buf + GPTH_Sign, "EFI PART" "\0\0\1\0" "\x5C\0\0", 16);	/* Signature, version (1.0) and size (92) */
		st_dword(buf + GPTH_PtBcc, ~bcc);				/* Table check sum */
		st_qword(buf + GPTH_CurLba, 1);					/* LBA of this header */
		st_qword(buf + GPTH_BakLba, sz_drv - 1);		/* LBA of another header */
		st_qword(buf + GPTH_FstLba, 2 + sz_pt);			/* LBA of first allocatable sector */
		st_qword(buf + GPTH_LstLba, s_bpt - 1);			/* LBA of last allocatable sector */
		st_dword(buf + GPTH_PteSize, SZ_GPTE);			/* Size of a table entry */
		st_dword(buf + GPTH_PtNum, GPT_ITEMS);			/* Number of table entries */
		st_dword(buf + GPTH_PtOfs, 2);					/* LBA of this table */
		rnd = make_rand(rnd, buf + GPTH_DskGuid, 16);	/* Disk GUID */
		for (i = 0, bcc= 0xFFFFFFFF; i < 92; bcc = crc32(bcc, buf[i++])) ;	/* Calculate header check sum */
		st_dword(buf + GPTH_Bcc, ~bcc);					/* Header check sum */
		if (disk_write(drv, buf, 1, 1) != RES_OK) return FR_DISK_ERR;

		/* Create secondary GPT header */
		st_qword(buf + GPTH_CurLba, sz_drv - 1);		/* LBA of this header */
		st_qword(buf + GPTH_BakLba, 1);					/* LBA of another header */
		st_qword(buf + GPTH_PtOfs, s_bpt);				/* LBA of this table */
		st_dword(buf + GPTH_Bcc, 0);
		for (i = 0, bcc= 0xFFFFFFFF; i < 92; bcc = crc32(bcc, buf[i++])) ;	/* Calculate header check sum */
		st_dword(buf + GPTH_Bcc, ~bcc);					/* Header check sum */
		if (disk_write(drv, buf, sz_drv - 1, 1) != RES_OK) return FR_DISK_ERR;

		/* Create protective MBR */
		mem_set(buf, 0, ss);
		mem_cpy(buf + MBR_Table, gpt_mbr, 16);			/* Create a GPT partition */
		st_word(buf + BS_55AA, 0xAA55);
		if (disk_write(drv, buf, 0, 1) != RES_OK) return FR_DISK_ERR;

	} else
#endif
	{					/* Create partitions in MBR */
		sz_drv32 = (DWORD)sz_drv;
		n_sc = N_SEC_TRACK;		/* Determine drive CHS without any consideration of the drive geometry */
		for (n_hd = 8; n_hd != 0 && sz_drv32 / n_hd / n_sc > 1024; n_hd *= 2) ;
		if (n_hd == 0) n_hd = 255;	/* Number of heads needs to be <256 */

		mem_set(buf, 0, FF_MAX_SS);	/* Clear MBR */
		pte = buf + MBR_Table;	/* Partition table in the MBR */
		for (i = 0, s_lba32 = n_sc; i < 4 && s_lba32 != 0 && s_lba32 < sz_drv32; i++, s_lba32 += n_lba32) {
			n_lba32 = (DWORD)plst[i];	/* Get partition size */
			if (n_lba32 <= 100) n_lba32 = (n_lba32 == 100) ? sz_drv32 : sz_drv32 / 100 * n_lba32;	/* Size in percentage? */
			if (s_lba32 + n_lba32 > sz_drv32 || s_lba32 + n_lba32 < s_lba32) n_lba32 = sz_drv32 - s_lba32;	/* Clip at drive size */
			if (n_lba32 == 0) break;	/* End of table or no sector to allocate? */

			st_dword(pte + PTE_StLba, s_lba32);		/* Start LBA */
			st_dword(pte + PTE_SizLba, n_lba32);	/* Number of sectors */
			pte[PTE_System] = (BYTE)sys;			/* System type */

			cy = (UINT)(s_lba32 / n_sc / n_hd);		/* Start cylinder */
			hd = (BYTE)(s_lba32 / n_sc % n_hd);		/* Start head */
			sc = (BYTE)(s_lba32 % n_sc + 1);		/* Start sector */
			pte[PTE_StHead] = hd;
			pte[PTE_StSec] = (BYTE)((cy >> 2 & 0xC0) | sc);
			pte[PTE_StCyl] = (BYTE)cy;

			cy = (UINT)((s_lba32 + n_lba32 - 1) / n_sc / n_hd);	/* End cylinder */
			hd = (BYTE)((s_lba32 + n_lba32 - 1) / n_sc % n_hd);	/* End head */
			sc = (BYTE)((s_lba32 + n_lba32 - 1) % n_sc + 1);	/* End sector */
			pte[PTE_EdHead] = hd;
			pte[PTE_EdSec] = (BYTE)((cy >> 2 & 0xC0) | sc);
			pte[PTE_EdCyl] = (BYTE)cy;

			pte += SZ_PTE;		/* Next entry */
		}

		st_word(buf + BS_55AA, 0xAA55);		/* MBR signature */
		if (disk_write(drv, buf, 0, 1) != RES_OK) return FR_DISK_ERR;	/* Write it to the MBR */
	}

	return FR_OK;
}



#endif
