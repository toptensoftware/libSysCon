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


#if FF_FS_EXFAT
#if FF_FS_MINIMIZE <= 1 || FF_FS_RPATH >= 2
/*------------------------------------------------------*/
/* exFAT: Get object information from a directory block */
/*------------------------------------------------------*/

void get_xfileinfo (
	BYTE* dirb,			/* Pointer to the direcotry entry block 85+C0+C1s */
	FILINFO* fno		/* Buffer to store the extracted file information */
)
{
	WCHAR wc, hs;
	UINT di, si, nc;

	/* Get file name from the entry block */
	si = SZDIRE * 2;	/* 1st C1 entry */
	nc = 0; hs = 0; di = 0;
	while (nc < dirb[XDIR_NumName]) {
		if (si >= MAXDIRB(FF_MAX_LFN)) { di = 0; break; }	/* Truncated directory block? */
		if ((si % SZDIRE) == 0) si += 2;		/* Skip entry type field */
		wc = ld_word(dirb + si); si += 2; nc++;	/* Get a character */
		if (hs == 0 && IsSurrogate(wc)) {	/* Is it a surrogate? */
			hs = wc; continue;	/* Get low surrogate */
		}
		wc = put_utf((DWORD)hs << 16 | wc, &fno->fname[di], FF_LFN_BUF - di);	/* Store it in API encoding */
		if (wc == 0) { di = 0; break; }	/* Buffer overflow or wrong encoding? */
		di += wc;
		hs = 0;
	}
	if (hs != 0) di = 0;					/* Broken surrogate pair? */
	if (di == 0) fno->fname[di++] = '?';	/* Inaccessible object name? */
	fno->fname[di] = 0;						/* Terminate the name */
	fno->altname[0] = 0;					/* exFAT does not support SFN */

	fno->fattrib = dirb[XDIR_Attr];			/* Attribute */
	fno->fsize = (fno->fattrib & AM_DIR) ? 0 : ld_qword(dirb + XDIR_FileSize);	/* Size */
	fno->ftime = ld_word(dirb + XDIR_ModTime + 0);	/* Time */
	fno->fdate = ld_word(dirb + XDIR_ModTime + 2);	/* Date */
}

#endif	/* FF_FS_MINIMIZE <= 1 || FF_FS_RPATH >= 2 */

#endif