#include "game_flags.h"

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
