#include "TYPES.H"

extern u8 GameFlagBytes[];

/* Whether one of the game's bit flags is set: ⚓️ shifts the flag's byte
   down to its bit where ☀️ masks it. */
s32 GameFlag_Test(s32 flag)
{
    s32 bit;
    u32 index;

    index = (u32)flag << 20;
    bit = flag & 7;
    flag = index >> 23;
    return (GameFlagBytes[flag] >> bit) & 1;
}
