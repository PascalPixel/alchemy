#include "game_flags.h"

u8 GameFlag_DecrementByte(s32 flag)
{
    u8 *bytes;

    bytes = (u8 *)0x02000040;
    flag = ((u32)flag << 0x14) >> 0x17;
    if (bytes[flag] != 0) {
        bytes[flag] = bytes[flag] + 0xFF;
    }
    return bytes[flag];
}
