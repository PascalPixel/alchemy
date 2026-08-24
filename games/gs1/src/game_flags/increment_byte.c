#include "game_flags.h"

u8 GameFlag_IncrementByte(s32 flag)
{
    u8 *flag_bytes;

    flag_bytes = (u8 *)0x02000040;
    flag = ((u32)flag << 0x14) >> 0x17;
    if (flag_bytes[flag] <= 0xFE) {
        flag_bytes[flag] = flag_bytes[flag] + 1;
    }
    return flag_bytes[flag];
}
