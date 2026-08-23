#include "game_flags.h"

void GameFlag_Clear(s32 flag)
{
    s32 mask;
    u8 *bytes;

    mask = ~(1 << (7 & flag));
    bytes = (u8 *)0x02000040;
    flag = ((u32)flag << 0x14) >> 0x17;
    bytes[flag] = (u8)(bytes[flag] & mask);
}
