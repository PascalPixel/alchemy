#ifndef ALCHEMY_FIXED_MATH_H
#define ALCHEMY_FIXED_MATH_H

/* 16.16 fixed-point constants and pixels, sine and cosine, and the system
   library's division. */

#include "types.h"

/* A signed 16.16 fixed-point number, as SGL types one: toFIXED(1) is 1.0. */
typedef Sint32 FIXED;
/* An angle, 65,536 to the turn, carried in 32 bits: SGL's ANGLE is 16 bits
   wide, and the callers here pass and keep 32. */
typedef Sint32 ANGLE;

/* The coordinate indices and their count, as SGL types them. */
enum { X, Y, Z, XYZ };

/* A direction or a scale in three 16.16 coordinates, as SGL types one. */
typedef FIXED VECTOR[XYZ];
/* A position in three 16.16 coordinates, as SGL types one. */
typedef FIXED POINT[XYZ];
/* A transform of four rows of three 16.16 words, the rotation's three and then
   the translation, as SGL types one but flat: FIXED[12], not [4][3]. */
typedef FIXED MATRIX[4 * XYZ];

/* A damping is a fraction of a speed kept each frame, out of DAMPING_SCALE:
   speed * damping / DAMPING_SCALE. */
#define DAMPING_SCALE 64

/* Whole pixels as 16.16 by a shift, unsigned for an unsigned value: kept only
   where toFIXED's signed multiply compiles to other instructions. */
#define INTtoFIXED(value)			 ((value) << 16)
/* The whole pixels of a 16.16 value, toFIXED() undone: an arithmetic shift, so
   a negative value rounds down. */
#define FIXEDtoINT(value)			 ((value) >> 16)
/* The whole-pixel half of a 16.16 coordinate, read in place as its high
   halfword. */
#define PIXEL_PART(v)				 (((Sint16 *)&(v))[1])
/* Twice a 16.16 value, as a whole number: FIXEDtoINT a shift short. */
#define FIXEDtoINT_TWICE(value)		 ((value) >> 15)
/* Four times a 16.16 value, as a whole number: FIXEDtoINT two shifts short. */
#define FIXEDtoINT_FOUR_TIMES(value) ((value) >> 14)

/* A whole or decimal number as 16.16, as SGL writes one: toFIXED(1) is 1.0 and
   toFIXED(0.8) is 52428. SGL multiplies by 65536.0; here the factor is the
   integer 65536, so a whole number stays integer arithmetic, and a multiply
   rather than a shift keeps toFIXED(-64) defined C. */
#define toFIXED(value) ((Sint32)((value) * 65536))

/* A whole turn in the angles that sySin and syCos take: DEGtoANG(90) is a
   quarter turn. */
#define TRIG_TURN		  (1 << 16)
/* An angle given in degrees, as SGL writes it: DEGtoANG(90) is a quarter
   turn. */
#define DEGtoANG(degrees) ((degrees) * TRIG_TURN / 360)
/* Keeps an angle within one turn: syRandom() & TRIG_ANGLE_MASK is a random
   angle. */
#define TRIG_ANGLE_MASK	  (TRIG_TURN - 1)

#endif
