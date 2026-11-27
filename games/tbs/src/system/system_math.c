/* @file */
/* Random numbers, angles and square roots. */
/* Part of system.c. */

/* Advances the game's random number generator (the C library's classic */
/* linear congruential one), stores the new seed and returns its bits 8 to */
/* 23, 0 to 65535. */
u32 syRandom(void) {
	enum { MULTIPLIER = 1103515245, INCREMENT = 12345 };
	u32 value = RandomSeed * MULTIPLIER + INCREMENT;

	RandomSeed = value;
	return (u16)(value >> 8);
}

/* Moves the point `position` by `radius` in direction `angle` on the ground */
/* plane: x by the cosine, z by the sine. */
void syVectorAddPolarOffset(FIXED radius, ANGLE angle, POINT position) {
	FIXED *coordinate = position;

	*coordinate++ += syMulFX(radius, sySin(angle + DEGtoANG(90)));
	coordinate++;
	*coordinate += syMulFX(radius, sySin(angle));
}

/* The entries of Math_TanTable in a quarter turn, which syAtan searches, */
/* the angle between neighbouring entries, and the tangent of the last one */
/* searched, in 8.8, beyond which syAtan gives a quarter turn. */
#define ATAN_ENTRIES	 256
#define ATAN_ENTRY_ANGLE (DEGtoANG(90) / ATAN_ENTRIES)
#define ATAN_RATIO_MAX	 64362

/* Returns the angle of the vector (x, y), from 0 to TRIG_TURN - 1, measured */
/* from the y axis: 0 along +y, a quarter turn along +x; (0, 0) gives 0. It */
/* binary-searches the tangent table to ATAN_ENTRY_ANGLE. */
ANGLE syAtan(FIXED x, FIXED y) {
	const u16 *table;
	s32 value;
	s32 ratio;
	s32 result;

	/* Handle axes without dividing by zero; the quadrant correction below */
	/* supplies the half-turn for a vector along -y. */
	if(x == 0) {
		result = 0;
	}
	else if(y == 0) {
		result = DEGtoANG(90);
	}
	else {
		/* Search only the first quadrant, with |x| / |y| in 8.8 fixed point. */
		ratio = y;
		if(ratio < 0)
			ratio = -ratio;
		value = x;
		if(value < 0)
			value = -value;

		ratio = (value << 8) / ratio;
		/* Beyond the last entry's tangent, use the quarter-turn limit instead
		 * of */
		/* the largest angle the eight search bits can represent. */
		result = DEGtoANG(90);
		if(ratio <= ATAN_RATIO_MAX) {
			/* Eight comparisons halve the entries searched each time. Each
			 * decides */
			/* one bit of the entry's index, from the highest down, and so of
			 * the */
			/* angle, from an eighth of a turn down to ATAN_ENTRY_ANGLE. */
			table = &Math_TanTable[ATAN_ENTRIES / 2 - 1];
			result = 0;

			value = *table;
			table -= ATAN_ENTRIES / 4;
			if(ratio > value) {
				result = TRIG_TURN / 8;
				table += ATAN_ENTRIES / 2;
			}
			value = *table;
			table -= ATAN_ENTRIES / 8;
			if(ratio > value) {
				result |= TRIG_TURN / 16;
				table += ATAN_ENTRIES / 4;
			}
			value = *table;
			table -= ATAN_ENTRIES / 16;
			if(ratio > value) {
				result |= TRIG_TURN / 32;
				table += ATAN_ENTRIES / 8;
			}
			value = *table;
			table -= ATAN_ENTRIES / 32;
			if(ratio > value) {
				result |= TRIG_TURN / 64;
				table += ATAN_ENTRIES / 16;
			}
			value = *table;
			table -= ATAN_ENTRIES / 64;
			if(ratio > value) {
				result |= TRIG_TURN / 128;
				table += ATAN_ENTRIES / 32;
			}
			value = *table;
			table -= ATAN_ENTRIES / 128;
			if(ratio > value) {
				result |= TRIG_TURN / 256;
				table += ATAN_ENTRIES / 64;
			}
			value = *table;
			table--;
			if(ratio > value) {
				result |= TRIG_TURN / 512;
				table += 2;
			}
			value = *table;
			if(ratio > value)
				result |= ATAN_ENTRY_ANGLE;
		}
	}

	/* Restore the signs after searching magnitudes alone. */
	if(y < 0)
		result = DEGtoANG(180) - result;
	if(x < 0)
		result = -result;

	return (u16)result;
}

/* The bits of a 32-bit value's square root. */
#define ROOT_BITS 16

/* Returns the integer square root of `value`, rounded down, a bit at a time; */
/* a negative `value` gives 0. Reached through the stub table. */
s32 syMathIntegerSqrt(s32 value) {
	s32 trial;
	s32 remainder;
	s32 bit;
	s32 result;

	/* remainder is value - result squared. */
	remainder = value;
	result = 0;
	for(bit = ROOT_BITS - 1; bit >= 0; bit -= 1) {
		trial = (result << (bit + 1)) + (1 << (bit * 2));
		if(trial <= remainder) {
			result |= 1 << bit;
			remainder -= trial;
		}
	}
	return result;
}

/* Square root of a 16.16 fixed-point value, in 16.16. The square root of */
/* the raw value is the result in 8.8, so the low eight bits are always 0. */
FIXED sySquartFX(FIXED value) { return sySquart(value) << 8; }

/* Returns the basic sprite palettes (BasicSpritePalettes). */
const u8 *syPaletteGetBasicSprCols(void) { return BasicSpritePalettes; }
