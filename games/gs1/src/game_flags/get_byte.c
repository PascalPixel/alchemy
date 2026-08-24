#include "game_flags.h"

u8 GameFlag_GetByte(s32 flag)
{
    u32 byte_index;

    byte_index = (u32)flag << 20;
    flag = (u32)byte_index >> 23;
    return GameFlagBytes[flag];
}
