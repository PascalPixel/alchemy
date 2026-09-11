#include "scene.h"
#include "game_flags.h"

/* game_flags/get_nibble.c */
s32 GameFlag_GetNibble(u32 flag)
{
    u32 shifted = flag << 20;
    u32 shift;
    u32 mask;
    s32 result;

    shift = flag & 4;
    mask = 15;
    flag = shifted >> 23;
    result = GameFlagBytes[flag];
    result &= mask << shift;
    return result >> shift;
}

/* game_flags/set_nibble.c */
void GameFlag_SetNibble(s32 flag, s32 value)
{
    s32 field_mask = 0xF;
    s32 shift = 4 & flag;
    s32 mask = field_mask << shift;
    u8 *bytes = (u8 *) 0x02000040;

    flag = ((u32)flag << 0x14) >> 0x17;
    bytes[flag] = (u8)((bytes[flag] & ~mask) |
                       ((value & field_mask) << shift));
}
