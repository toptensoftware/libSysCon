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


#if FF_FS_RPATH >= 1

/*-----------------------------------------------------------------------*/
/* Change Current Directory or Current Drive, Get Current Directory      */
/*-----------------------------------------------------------------------*/

FRESULT f_chdrive (
	const TCHAR* path		/* Drive number to set */
)
{
	int vol;


	/* Get logical drive number */
	vol = get_ldnumber(&path);
	if (vol < 0) return FR_INVALID_DRIVE;
	CurrVol = (BYTE)vol;	/* Set it as current volume */

	return FR_OK;
}

#endif
