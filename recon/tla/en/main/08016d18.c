#include "TYPES.H"
extern u8 GameFlagBytes[];

/* game_flags/test.c */

void GameFlag_ClearBit(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;

    flag_mask = ~(1 << (7 & flag));
    flag_bytes = (u8 *)GameFlagBytes;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = (u8)(flag_bytes[flag] & flag_mask);
}
