/*
   tilt.h  -  (c) 1999 Till Harbaum

   palm tilt sensor interface

   t.harbaum@tu-bs.de
*/
#include "typedefs.h"

#ifndef __TILT_H__
#define __TILT_H__

#define TILT_WRONG_OS  3
#define TILT_NO_SENSOR 4
#define TILT_HW_BUSY   5

/* tilt interface function declarations */
Err TiltLibOpen(UInt16 refNum);
Err TiltLibClose(UInt16 refNum, UInt16 *numappsP);
Err TiltLibZero(UInt16 refNum);
Err TiltLibGet(UInt16 refNum, UInt16 *x, UInt16 *y);
Err TiltLibGetAbs(UInt16 refNum, UInt16 *x, UInt16 *y);

#endif

