// syUnitGet and the game flags: bits, bytes and nibbles. Part of system.c.

#include "game/game_flags.h"

// Returns the unit of unitId: ISAAC to PIERS the characters, the ENEMY_BATTLE_SIZE from
// FIRST_ENEMY_UNIT the enemies of the current battle; NULL for anything else.
UNIT *syUnitGet(Uint32 unitId)
{
	UNIT *states;
	UNIT *unit;

	if (unitId <= PIERS)
	{
		states = CharacterUnits;
		unit   = &states[unitId];
		return unit;
	}
	if (unitId - FIRST_ENEMY_UNIT < ENEMY_BATTLE_SIZE
	    && (states = RamHeapSlots->enemyUnits) != NULL)
	{
		unit = &states[unitId - FIRST_ENEMY_UNIT];
		return unit;
	}
	return NULL;
}

// The game flags: 4096 bits in GameFlagBytes. A flag number's low three bits pick the bit and the
// next nine the byte; byte and nibble flags use the same numbering. Bits above the twelfth are
// dropped (GAME_FLAG_BYTE), so a number past 4095 wraps onto a flag below it rather than reaching
// other memory.

// Returns 1 when `flags` is set and 0 when it is clear.
Sint32 syGameFlagGet(Sint32 flag)
{
	Sint32 bit = flag & 7;

	flag       = GAME_FLAG_BYTE(flag);
	return (GameFlagBytes[flag] >> bit) & 1;
}

// Sets `flags` and returns its byte number.
Sint32 syGameFlagSet(Sint32 flag)
{
	Sint32 bit         = GAME_FLAG_BIT(flag);
	Uint8 *flagsBytes  = GameFlagBytes;
	flag               = GAME_FLAG_BYTE(flag);

	flagsBytes[flag]  |= bit;
	return flag;
}

// Clears `flags`.
void syGameFlagClear(Sint32 flag)
{
	Sint32 bit         = GAME_FLAG_BIT(flag);
	Uint8 *flagsBytes  = GameFlagBytes;
	flag               = GAME_FLAG_BYTE(flag);

	flagsBytes[flag]  &= ~bit;
}

// Unused: flips `flags` and returns its new value.
Uint32 syGameFlagToggle(Sint32 flag)
{
	Sint32 bit         = GAME_FLAG_BIT(flag);
	Uint8 *flagsBytes  = GameFlagBytes;
	flag               = GAME_FLAG_BYTE(flag);

	flagsBytes[flag]  ^= bit;
	if (flagsBytes[flag] & bit)
		return 1;
	return 0;
}

// Returns the whole byte `flags` is in, a counter kept among the flags.
Sint32 syGameFlagGetByte(Sint32 flag)
{
	flag = GAME_FLAG_BYTE(flag);
	return GameFlagBytes[flag];
}

// Sets the whole byte `flags` is in.
Uint32 syGameFlagSetByte(Uint32 flag, Uint8 value)
{
	flag                = GAME_FLAG_BYTE(flag);
	GameFlagBytes[flag] = value;
	return flag;
}

// The most a counter byte among the flags holds.
#define GAME_FLAG_BYTE_MAX 255

// Unused: adds 1 to the byte `flags` is in, stopping at GAME_FLAG_BYTE_MAX, and returns it.
Uint8 syGameFlagIncrementByte(Sint32 flag)
{
	Uint8 *flagsBytes = GameFlagBytes;
	flag              = GAME_FLAG_BYTE(flag);

	if (flagsBytes[flag] < GAME_FLAG_BYTE_MAX)
		flagsBytes[flag]++;
	return flagsBytes[flag];
}

// Unused: takes 1 from the byte `flags` is in, stopping at 0, and returns it.
Uint8 syGameFlagDecrementByte(Sint32 flag)
{
	Uint8 *flagsBytes = GameFlagBytes;
	flag              = GAME_FLAG_BYTE(flag);

	if (flagsBytes[flag] != 0)
		flagsBytes[flag]--;
	return flagsBytes[flag];
}

// The bits of a nibble flag, low or high in its byte by its number's bit 2.
#define GAME_FLAG_NIBBLE_MASK 0xf

// Unused: returns the four bits a flag is in, 0 to 15.
Sint32 syGameFlagGetNibble(Uint32 flag)
{
	Sint32 shift = flag & 4;
	Sint32 mask  = GAME_FLAG_NIBBLE_MASK << shift;
	flag         = GAME_FLAG_BYTE(flag);

	return (GameFlagBytes[flag] & mask) >> shift;
}

// Unused: sets the four bits `flags` is in.
void syGameFlagSetNibble(Sint32 flag, Sint32 value)
{
	Sint32 nibble     = GAME_FLAG_NIBBLE_MASK;
	Sint32 shift      = flag & 4;
	Sint32 mask       = nibble << shift;
	Uint8 *flagsBytes = GameFlagBytes;
	flag              = GAME_FLAG_BYTE(flag);

	flagsBytes[flag]  = (flagsBytes[flag] & ~mask) | ((value & nibble) << shift);
}
