/* @file */
/* The game flags: bits, bytes and nibbles. */
/* Part of game.c. */

/* The game flags: 4096 bits in GameFlagBytes. A flag number's low three bits */
/* pick the bit and the next nine the byte; byte and nibble flags use the same
 */
/* numbering. Bits above the twelfth are dropped (GAME_FLAG_BYTE), so a number
 */
/* past 4095 wraps onto a flag below it rather than reaching other memory. */

/* Returns 1 when `flag` is set. Every module tests flags through it, as */
/* gmFlagGetStub, and the events directly or through event.h. */
s32 gmFlagGet(s32 flag) {
	s32 bit = GAME_FLAG_BIT(flag);

	flag = GAME_FLAG_BYTE(flag);
	if(GameFlagBytes[flag] & bit)
		return 1;
	return 0;
}

/* Sets `flag` and returns its byte number. */
s32 gmFlagSet(s32 flag) {
	s32 bit = GAME_FLAG_BIT(flag);
	u8 *flags = GameFlagBytes;
	flag = GAME_FLAG_BYTE(flag);

	flags[flag] |= bit;
	return flag;
}

/* Clears `flag`. */
void gmFlagClear(s32 flag) {
	s32 bit = GAME_FLAG_BIT(flag);
	u8 *flags = GameFlagBytes;
	flag = GAME_FLAG_BYTE(flag);

	flags[flag] &= ~bit;
}

/* Flips `flag` and returns its new value. Only game.s's bare stub reaches */
/* it; nothing calls it by name. */
u32 gmFlagToggle(s32 flag) {
	s32 bit = GAME_FLAG_BIT(flag);
	u8 *flags = GameFlagBytes;
	flag = GAME_FLAG_BYTE(flag);

	flags[flag] ^= bit;
	if(flags[flag] & bit)
		return 1;
	return 0;
}

/* Returns the whole byte `flag` is in, a counter kept among the flags. */
s32 gmFlagGetByte(s32 flag) {
	flag = GAME_FLAG_BYTE(flag);

	return GameFlagBytes[flag];
}

/* Sets the whole byte `flag` is in. */
void gmFlagSetByte(s32 flag, s32 value) {
	flag = GAME_FLAG_BYTE(flag);

	GameFlagBytes[flag] = value;
}

/* The most a counter byte among the flags holds. */
#define GAME_FLAG_BYTE_MAX 255

/* Adds 1 to the byte `flag` is in, stopping at GAME_FLAG_BYTE_MAX, and returns
 */
/* it. Only game.s's bare stub reaches it; nothing calls it by name. */
u8 gmFlagIncrementByte(s32 flag) {
	u8 *flags = GameFlagBytes;
	flag = GAME_FLAG_BYTE(flag);

	if(flags[flag] < GAME_FLAG_BYTE_MAX)
		flags[flag]++;
	return flags[flag];
}

/* Takes 1 from the byte `flag` is in, stopping at 0, and returns it. Only */
/* game.s's bare stub reaches it; nothing calls it by name. */
u8 gmFlagDecrementByte(s32 flag) {
	u8 *flags = GameFlagBytes;
	flag = GAME_FLAG_BYTE(flag);

	if(flags[flag] != 0)
		flags[flag]--;
	return flags[flag];
}

/* The bits of a nibble flag, low or high in its byte by its number's bit 2. */
#define GAME_FLAG_NIBBLE_MASK 0xf

/* Returns the four bits a flag is in, 0 to 15. */
s32 gmFlagGetNibble(u32 flag) {
	s32 shift = flag & 4;
	s32 mask = GAME_FLAG_NIBBLE_MASK << shift;
	flag = GAME_FLAG_BYTE(flag);

	return (GameFlagBytes[flag] & mask) >> shift;
}

/* Sets the four bits `flag` is in. Only game.s's bare stub reaches it; */
/* nothing calls it by name. */
void gmFlagSetNibble(s32 flag, s32 value) {
	s32 nibble = GAME_FLAG_NIBBLE_MASK;
	s32 shift = flag & 4;
	s32 mask = nibble << shift;
	u8 *flags = GameFlagBytes;
	flag = GAME_FLAG_BYTE(flag);

	flags[flag] = (flags[flag] & ~mask) | ((value & nibble) << shift);
}
