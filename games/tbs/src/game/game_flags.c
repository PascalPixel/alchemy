/* The game flags: bits, bytes and nibbles. Part of game.c. */

/* The game flags: 4096 bits in GameFlagBytes. A flag number's low three bits
   pick the bit and the next nine the byte; byte and nibble flags use the same
   numbering. Bits above the twelfth are dropped (GAME_FLAG_BYTE), so a number
   past 4095 wraps onto a flag below it rather than reaching other memory. */

/* Returns 1 when `flg` is set. Every module tests flags through it, as
   gmFlagGetStub, and the scripts directly or through script.h. */
Bool gmFlagGet(Sint32 flg)
{
	Sint32 bit = GAME_FLAG_BIT(flg);

	flg = GAME_FLAG_BYTE(flg);
	if(GameFlagBytes[flg] & bit)
		return TRUE;
	return FALSE;
}

/* Sets `flg` and returns its byte number. */
Sint32 gmFlagSet(Sint32 flg)
{
	Sint32 bit = GAME_FLAG_BIT(flg);
	Uint8 *flgBytes = GameFlagBytes;
	flg = GAME_FLAG_BYTE(flg);

	flgBytes[flg] |= bit;
	return flg;
}

/* Clears `flg`. */
void gmFlagClear(Sint32 flg)
{
	Sint32 bit = GAME_FLAG_BIT(flg);
	Uint8 *flgBytes = GameFlagBytes;
	flg = GAME_FLAG_BYTE(flg);

	flgBytes[flg] &= ~bit;
}

/* Flips `flg` and returns its new value. Only game.s's bare stub reaches it;
   nothing calls it by name. */
Uint32 gmFlagToggle(Sint32 flg)
{
	Sint32 bit = GAME_FLAG_BIT(flg);
	Uint8 *flgBytes = GameFlagBytes;
	flg = GAME_FLAG_BYTE(flg);

	flgBytes[flg] ^= bit;
	if(flgBytes[flg] & bit)
		return 1;
	return 0;
}

/* Returns the whole byte `flg` is in, a counter kept among the flags. */
Sint32 gmFlagGetByte(Sint32 flg)
{
	flg = GAME_FLAG_BYTE(flg);

	return GameFlagBytes[flg];
}

/* Sets the whole byte `flg` is in. */
void gmFlagSetByte(Sint32 flg, Sint32 value)
{
	flg = GAME_FLAG_BYTE(flg);

	GameFlagBytes[flg] = value;
}

/* The most a counter byte among the flags holds. */
#define GAME_FLAG_BYTE_MAX 255

/* Adds 1 to the byte `flg` is in, stopping at GAME_FLAG_BYTE_MAX, and returns
   it. Only game.s's bare stub reaches it; nothing calls it by name. */
Uint8 gmFlagIncrementByte(Sint32 flg)
{
	Uint8 *flgBytes = GameFlagBytes;
	flg = GAME_FLAG_BYTE(flg);

	if(flgBytes[flg] < GAME_FLAG_BYTE_MAX)
		flgBytes[flg]++;
	return flgBytes[flg];
}

/* Takes 1 from the byte `flg` is in, stopping at 0, and returns it. Only
   game.s's bare stub reaches it; nothing calls it by name. */
Uint8 gmFlagDecrementByte(Sint32 flg)
{
	Uint8 *flgBytes = GameFlagBytes;
	flg = GAME_FLAG_BYTE(flg);

	if(flgBytes[flg] != 0)
		flgBytes[flg]--;
	return flgBytes[flg];
}

/* The bits of a nibble flag, low or high in its byte by its number's bit 2. */
#define GAME_FLAG_NIBBLE_MASK 0xf

/* Returns the four bits a flag is in, 0 to 15. */
Sint32 gmFlagGetNibble(Uint32 flg)
{
	Sint32 shift = flg & 4;
	Sint32 mask = GAME_FLAG_NIBBLE_MASK << shift;
	flg = GAME_FLAG_BYTE(flg);

	return (GameFlagBytes[flg] & mask) >> shift;
}

/* Sets the four bits `flg` is in. Only game.s's bare stub reaches it; nothing
   calls it by name. */
void gmFlagSetNibble(Sint32 flg, Sint32 value)
{
	Sint32 nibble = GAME_FLAG_NIBBLE_MASK;
	Sint32 shift = flg & 4;
	Sint32 mask = nibble << shift;
	Uint8 *flgBytes = GameFlagBytes;
	flg = GAME_FLAG_BYTE(flg);

	flgBytes[flg] = (flgBytes[flg] & ~mask) | ((value & nibble) << shift);
}
