#include "game_flags.h"

u8 GameFlag_DecrementByte(s32 flag)
{
    u8 *flag_bytes;

    flag_bytes = (u8 *)0x02000040;
    flag = ((u32)flag << 0x14) >> 0x17;
    if (flag_bytes[flag] != 0) {
        flag_bytes[flag] = flag_bytes[flag] + 0xFF;
    }
    return flag_bytes[flag];
}
