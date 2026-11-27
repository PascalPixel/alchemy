/* Random numbers, angles and square roots. Part of system.c. */

#include "system/random.c"

/* Moves the point `pos` by `radius` in direction `ang` on the ground
   plane: x by the cosine, z by the sine. */
void syVectorAddPolarOffset(FIXED radius, ANGLE ang, POINT pos)
{
	*pos++ += syMulFX(radius, sySin(ang + DEGtoANG(90)));
	pos++;
	*pos += syMulFX(radius, sySin(ang));
}

/* The entries of MathTanTable in a quarter turn, which syAtan searches, the
   angle between neighbouring entries, and the tangent of the last one searched,
   in 8.8, beyond which syAtan gives a quarter turn. */
#define ATAN_ENTRIES	 256
#define ATAN_ENTRY_ANGLE (DEGtoANG(90) / ATAN_ENTRIES)
#define ATAN_RATIO_MAX	 64362

/* Returns the angle of the vector (x, y), from 0 to TRIG_TURN - 1, measured
   from the y axis: 0 along +y, a quarter turn along +x; (0, 0) gives 0. It
   binary-searches the tangent table to ATAN_ENTRY_ANGLE. */
ANGLE syAtan(FIXED x, FIXED y)
{
	const Uint16 *tbl;
	Sint32		  value;
	Sint32		  ratio;
	Sint32		  result;
	Uint32		  bit;

	/* Handle axes without dividing by zero; the quadrant correction below
	   supplies the half-turn for a vector along -y. */
	if(x == 0) {
		result = 0;
	} else if(y == 0) {
		result = DEGtoANG(90);
	} else {
		/* Search only the first quadrant, with |x| / |y| in 8.8 fixed point. */
		ratio = y;
		if(ratio < 0)
			ratio = -ratio;
		value = x;
		if(value < 0)
			value = -value;

		ratio = syIntegerDivideUnsigned(value << 8, ratio);
		result = DEGtoANG(90);
		/* Beyond the last entry's tangent, use the quarter-turn limit instead
		   of the largest angle the eight search bits can represent. */
		if(ratio <= ATAN_RATIO_MAX) {
			/* Eight comparisons halve the entries searched each time. Each
			   decides one bit of the entry's index, from the highest down, and
			   so of the angle, from an eighth of a turn down to
			   ATAN_ENTRY_ANGLE. */
			tbl = &MathTanTable[ATAN_ENTRIES / 2 - 1];
			result = 0;
			bit = DEGtoANG(45);

			value = *tbl;
			tbl -= ATAN_ENTRIES / 4;
			if(ratio > value) {
				result += bit;
				tbl += ATAN_ENTRIES / 2;
			}
			value = *tbl;
			bit >>= 1;
			tbl -= ATAN_ENTRIES / 8;
			if(ratio > value) {
				result += bit;
				tbl += ATAN_ENTRIES / 4;
			}
			value = *tbl;
			bit >>= 1;
			tbl -= ATAN_ENTRIES / 16;
			if(ratio > value) {
				result += bit;
				tbl += ATAN_ENTRIES / 8;
			}
			value = *tbl;
			bit >>= 1;
			tbl -= ATAN_ENTRIES / 32;
			if(ratio > value) {
				result += bit;
				tbl += ATAN_ENTRIES / 16;
			}
			value = *tbl;
			bit >>= 1;
			tbl -= ATAN_ENTRIES / 64;
			if(ratio > value) {
				result += bit;
				tbl += ATAN_ENTRIES / 32;
			}
			value = *tbl;
			bit >>= 1;
			tbl -= ATAN_ENTRIES / 128;
			if(ratio > value) {
				result += bit;
				tbl += ATAN_ENTRIES / 64;
			}
			value = *tbl;
			tbl--;
			if(ratio > value) {
				result += 2 * ATAN_ENTRY_ANGLE;
				tbl += 2;
			}
			value = *tbl;
			if(ratio > value)
				result += ATAN_ENTRY_ANGLE;
		}
	}

	/* Restore the signs after searching magnitudes alone. */
	if(y < 0)
		result = DEGtoANG(180) - result;
	if(x < 0)
		result = -result;

	return (Uint16)result;
}

/* The bits of a 32-bit value's square root. */
#define ROOT_BITS 16

#include "system/squart.c"

/* Returns the basic sprite palettes (SystemBasicColorPalette). */
Sint32 syPaletteGetBasicSprCols(void)
{
	return (Sint32)SystemBasicColorPalette;
}
