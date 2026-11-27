// The game flags: bits, bytes and nibbles. Part of game.c.

// The game flags: 4096 bits in GameFlagBytes. A flag number's low three bits pick the bit and the
// next nine the byte; byte and nibble flags use the same numbering. Bits above the twelfth are
// dropped (GAME_FLAG_BYTE), so a number past 4095 wraps onto a flag below it rather than reaching
// other memory.

// Returns 1 when `flags` is set. Every module tests flags through it, as gmFlagGetStub, and the
// scripts directly or through script.h.
Bool gmFlagGet(Sint32 flag)
{
    Sint32 bit = GAME_FLAG_BIT(flag);

    flag       = GAME_FLAG_BYTE(flag);
    if (GameFlagBytes[flag] & bit)
        return TRUE;
    return FALSE;
}

// Sets `flags` and returns its byte number.
Sint32 gmFlagSet(Sint32 flag)
{
    Sint32 bit         = GAME_FLAG_BIT(flag);
    Uint8 *flagsBytes  = GameFlagBytes;
    flag               = GAME_FLAG_BYTE(flag);

    flagsBytes[flag]  |= bit;
    return flag;
}

// Clears `flags`.
void gmFlagClear(Sint32 flag)
{
    Sint32 bit         = GAME_FLAG_BIT(flag);
    Uint8 *flagsBytes  = GameFlagBytes;
    flag               = GAME_FLAG_BYTE(flag);

    flagsBytes[flag]  &= ~bit;
}

// Flips `flags` and returns its new value. Only game.s's bare stub reaches it; nothing calls it by
// name.
Uint32 gmFlagToggle(Sint32 flag)
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
Sint32 gmFlagGetByte(Sint32 flag)
{
    flag = GAME_FLAG_BYTE(flag);

    return GameFlagBytes[flag];
}

// Sets the whole byte `flags` is in.
void gmFlagSetByte(Sint32 flag, Sint32 value)
{
    flag                = GAME_FLAG_BYTE(flag);

    GameFlagBytes[flag] = value;
}

// The most a counter byte among the flags holds.
#define GAME_FLAG_BYTE_MAX 255

// Adds 1 to the byte `flags` is in, stopping at GAME_FLAG_BYTE_MAX, and returns it. Only game.s's
// bare stub reaches it; nothing calls it by name.
Uint8 gmFlagIncrementByte(Sint32 flag)
{
    Uint8 *flagsBytes = GameFlagBytes;
    flag              = GAME_FLAG_BYTE(flag);

    if (flagsBytes[flag] < GAME_FLAG_BYTE_MAX)
        flagsBytes[flag]++;
    return flagsBytes[flag];
}

// Takes 1 from the byte `flags` is in, stopping at 0, and returns it. Only game.s's bare stub
// reaches it; nothing calls it by name.
Uint8 gmFlagDecrementByte(Sint32 flag)
{
    Uint8 *flagsBytes = GameFlagBytes;
    flag              = GAME_FLAG_BYTE(flag);

    if (flagsBytes[flag] != 0)
        flagsBytes[flag]--;
    return flagsBytes[flag];
}

// The bits of a nibble flag, low or high in its byte by its number's bit 2.
#define GAME_FLAG_NIBBLE_MASK 0xf

// Returns the four bits a flag is in, 0 to 15.
Sint32 gmFlagGetNibble(Uint32 flag)
{
    Sint32 shift = flag & 4;
    Sint32 mask  = GAME_FLAG_NIBBLE_MASK << shift;
    flag         = GAME_FLAG_BYTE(flag);

    return (GameFlagBytes[flag] & mask) >> shift;
}

// Sets the four bits `flags` is in. Only game.s's bare stub reaches it; nothing calls it by name.
void gmFlagSetNibble(Sint32 flag, Sint32 value)
{
    Sint32 nibble     = GAME_FLAG_NIBBLE_MASK;
    Sint32 shift      = flag & 4;
    Sint32 mask       = nibble << shift;
    Uint8 *flagsBytes = GameFlagBytes;
    flag              = GAME_FLAG_BYTE(flag);

    flagsBytes[flag]  = (flagsBytes[flag] & ~mask) | ((value & nibble) << shift);
}
