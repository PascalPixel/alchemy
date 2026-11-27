/* syUnitGet and the game flags: bits, bytes and nibbles. Part of system.c. */

#include "game/game_flags.h"

/* Returns the unit of unitId: ISAAC to PIERS the characters, the
   ENEMY_BATTLE_SIZE from FIRST_ENEMY_UNIT the enemies of the current battle;
   NULL for anything else. */
UNIT *syUnitGet(Uint32 unitId)
{
	UNIT *states;
	UNIT *unit;

	if(unitId <= PIERS) {
		states = CharacterUnits;
		unit = &states[unitId];
		return unit;
	}
	if(unitId - FIRST_ENEMY_UNIT < ENEMY_BATTLE_SIZE &&
		(states = Ram_HeapSlots->enemyUnits) != NULL) {
		unit = &states[unitId - FIRST_ENEMY_UNIT];
		return unit;
	}
	return NULL;
}

/* The game flags: 4096 bits in GameFlagBytes. A flag number's low three bits
   pick the bit and the next nine the byte; byte and nibble flags use the same
   numbering. Bits above the twelfth are dropped (GAME_FLAG_BYTE), so a number
   past 4095 wraps onto a flag below it rather than reaching other memory. */

/* Returns 1 when `flg` is set and 0 when it is clear. */
Sint32 syGameFlagGet(Sint32 flg)
{
	Sint32 bit = flg & 7;

	flg = GAME_FLAG_BYTE(flg);
	return (GameFlagBytes[flg] >> bit) & 1;
}

/* Sets `flg` and returns its byte number. */
Sint32 syGameFlagSet(Sint32 flg)
{
	Sint32 bit = GAME_FLAG_BIT(flg);
	Uint8 *flgBytes = GameFlagBytes;
	flg = GAME_FLAG_BYTE(flg);

	flgBytes[flg] |= bit;
	return flg;
}

/* Clears `flg`. */
void syGameFlagClear(Sint32 flg)
{
	Sint32 bit = GAME_FLAG_BIT(flg);
	Uint8 *flgBytes = GameFlagBytes;
	flg = GAME_FLAG_BYTE(flg);

	flgBytes[flg] &= ~bit;
}

/* Unused: flips `flg` and returns its new value. */
Uint32 syGameFlagToggle(Sint32 flg)
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
Sint32 syGameFlagGetByte(Sint32 flg)
{
	flg = GAME_FLAG_BYTE(flg);
	return GameFlagBytes[flg];
}

/* Sets the whole byte `flg` is in. */
Uint32 syGameFlagSetByte(Uint32 flg, Uint8 value)
{
	flg = GAME_FLAG_BYTE(flg);
	GameFlagBytes[flg] = value;
	return flg;
}

/* The most a counter byte among the flags holds. */
#define GAME_FLAG_BYTE_MAX 255

/* Unused: adds 1 to the byte `flg` is in, stopping at GAME_FLAG_BYTE_MAX, and
   returns it. */
Uint8 syGameFlagIncrementByte(Sint32 flg)
{
	Uint8 *flgBytes = GameFlagBytes;
	flg = GAME_FLAG_BYTE(flg);

	if(flgBytes[flg] < GAME_FLAG_BYTE_MAX)
		flgBytes[flg]++;
	return flgBytes[flg];
}

/* Unused: takes 1 from the byte `flg` is in, stopping at 0, and returns it. */
Uint8 syGameFlagDecrementByte(Sint32 flg)
{
	Uint8 *flgBytes = GameFlagBytes;
	flg = GAME_FLAG_BYTE(flg);

	if(flgBytes[flg] != 0)
		flgBytes[flg]--;
	return flgBytes[flg];
}

/* The bits of a nibble flag, low or high in its byte by its number's bit 2. */
#define GAME_FLAG_NIBBLE_MASK 0xf

/* Unused: returns the four bits a flag is in, 0 to 15. */
Sint32 syGameFlagGetNibble(Uint32 flg)
{
	Sint32 shift = flg & 4;
	Sint32 mask = GAME_FLAG_NIBBLE_MASK << shift;
	flg = GAME_FLAG_BYTE(flg);

	return (GameFlagBytes[flg] & mask) >> shift;
}

/* Unused: sets the four bits `flg` is in. */
void syGameFlagSetNibble(Sint32 flg, Sint32 value)
{
	Sint32 nibble = GAME_FLAG_NIBBLE_MASK;
	Sint32 shift = flg & 4;
	Sint32 mask = nibble << shift;
	Uint8 *flgBytes = GameFlagBytes;
	flg = GAME_FLAG_BYTE(flg);

	flgBytes[flg] = (flgBytes[flg] & ~mask) | ((value & nibble) << shift);
}
