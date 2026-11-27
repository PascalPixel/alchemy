/* sySquart and sySquartFX, the same in both games. Both games'
   system/system_math.c include it where the functions stand. */

/* Returns the integer square root of `value`, rounded down, a bit at a time; a
   negative `value` gives 0. Reached through the stub table. */
Sint32 sySquart(Sint32 value)
{
	Sint32 trial;
	Sint32 remainder;
	Sint32 bit;
	Sint32 result;

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

/* Square root of a 16.16 fixed-point value, in 16.16. The square root of the
   raw value is the result in 8.8, so the low eight bits are always 0. */
FIXED sySquartFX(FIXED value)
{
	return sySquartIWRAM(value) << 8;
}
