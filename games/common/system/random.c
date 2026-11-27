/* syRandom, the same in both games. Both games' system/system_math.c include
   it where the function stands. */

/* Advances the game's random number generator (the C library's classic linear
   congruential one), stores the new seed and returns its bits 8 to 23, 0 to
   65535. Called throughout the game. */
Uint32 syRandom(void)
{
	enum { MULTIPLIER = 1103515245, INCREMENT = 12345 };

	Uint32 value = RandomSeed * MULTIPLIER + INCREMENT;

	RandomSeed = value;
	return (Uint16)(value >> 8);
}
