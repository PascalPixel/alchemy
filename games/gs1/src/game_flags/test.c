#include "game_flags.h"

s32 GameFlag_Test(s32 flag)
{
    s32 mask;
    u8 *bytes;
    s32 value;

    mask = 1 << (7 & flag);
    flag = ((u32)flag << 0x14) >> 0x17;
    bytes = (u8 *)0x02000040;
    value = bytes[flag] & mask;
    return (s32)((u32)((0 - value) | value) >> 0x1F);
}
