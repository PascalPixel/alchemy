#include "game_flags.h"

s32 GameFlag_Test(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;
    s32 flag_value;

    flag_mask = 1 << (7 & flag);
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes = (u8 *)0x02000040;
    flag_value = flag_bytes[flag] & flag_mask;
    return (s32)((u32)((0 - flag_value) | flag_value) >> 0x1F);
}
