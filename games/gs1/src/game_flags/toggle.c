#include "game_flags.h"

u32 GameFlag_Toggle(s32 flag)
{
    s32 mask;
    u8 *bytes;
    s32 value;

    mask = 1 << (7 & flag);
    bytes = (u8 *)0x02000040;
    flag = ((u32)flag << 0x14) >> 0x17;
    bytes[flag] = (u8)(bytes[flag] ^ mask);
    value = bytes[flag] & mask;
    return (u32)((0 - value) | value) >> 31;
}
