#ifndef ALCHEMY_FIXED_MATH_H
#define ALCHEMY_FIXED_MATH_H

/* @file */
/* 16.16 fixed-point constants and pixels, sine and cosine, and the system */
/* library's division. */

#include "types.h"

/* A signed 16.16 fixed-point number, as SGL types one: FIXED_ONE is 1.0. */
typedef s32 FIXED;
/* An angle, 65,536 to the turn, carried in 32 bits: SGL's ANGLE is 16 bits */
/* wide, and the callers here pass and keep 32. */
typedef s32 ANGLE;

/* The coordinate indices and their count, as SGL types them. */
enum { X, Y, Z, XYZ };
/* A direction or a scale in three 16.16 coordinates, as SGL types one. */
typedef FIXED VECTOR[XYZ];
/* A position in three 16.16 coordinates, as SGL types one. */
typedef FIXED POINT[XYZ];
/* A transform of four rows of three 16.16 words, the rotation's three and */
/* then the translation, as SGL types one but flat: FIXED[12], not [4][3]. */
typedef FIXED MATRIX[4 * XYZ];

/* One in the game's signed 16.16 fixed-point format. */
#define FIXED_ONE (1 << 16)

/* A damping is a fraction of a speed kept each frame, out of DAMPING_SCALE: */
/* speed * damping / DAMPING_SCALE. */
#define DAMPING_SCALE 64

/* Whole pixels as 16.16, for arithmetic on a stored position or speed. */
#define INTtoFIXED(value) ((value) << 16)
/* The whole pixels of a 16.16 value, toFIXED() undone: an arithmetic shift, so
 */
/* a negative value rounds down. */
#define FIXEDtoINT(value) ((value) >> 16)
/* Twice a 16.16 value, as a whole number: FIXEDtoINT a shift short. */
#define FIXEDtoINT_TWICE(value) ((value) >> 15)

/* A whole or decimal number as 16.16, for the macros that take plain pixels */
/* and decimal speeds: toFIXED(0.8) is 52428. It multiplies where toFIXED() */
/* shifts, so toFIXED(-64) is defined C. */
#define toFIXED(value) ((s32)((value) * FIXED_ONE))

/* A whole turn in the angles that sySin, syCos and the transform's */
/* turns take, 65,536 to the turn. */
#define TRIG_TURN (1 << 16)
/* An angle given in degrees, as SGL writes it: DEGtoANG(90) is a quarter turn.
 */
#define DEGtoANG(degrees) ((degrees) * TRIG_TURN / 360)
/* Keeps an angle within one turn: syRandom() & TRIG_ANGLE_MASK is a random */
/* angle. */
#define TRIG_ANGLE_MASK (TRIG_TURN - 1)

#endif
