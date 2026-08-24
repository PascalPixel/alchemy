#include "game_flags.h"

u32 GameFlag_Toggle(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;
    s32 flag_value;

    flag_mask = 1 << (7 & flag);
    flag_bytes = (u8 *)0x02000040;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = (u8)(flag_bytes[flag] ^ flag_mask);
    flag_value = flag_bytes[flag] & flag_mask;
    return (u32)((0 - flag_value) | flag_value) >> 31;
}
