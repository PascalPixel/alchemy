#include "game_flags.h"

u32 GameFlag_SetByte(u32 flag, u8 value)
{
    u32 shifted = flag << 20;
    flag = shifted >> 23;
    GameFlagBytes[flag] = value;
    return flag;
}
