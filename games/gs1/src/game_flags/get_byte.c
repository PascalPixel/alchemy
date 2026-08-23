#include "game_flags.h"

u8 GameFlag_GetByte(s32 flag)
{
    u32 index;

    index = (u32)flag << 20;
    flag = (u32)index >> 23;
    return GameFlagBytes[flag];
}
