/*
 * Draft: GameFlag_SetNibble does not yet match; 4 halfwords differ from ☀️'s C, first at +0xc (lsrs r6, r3, #23).
 * Links as recon/tla/raw/08016dd0.s.
 */
#include "TYPES.H"

extern u8 GameFlagBytes[];

extern u8 GameFlagBytes[];

/* game_flags/set_nibble.c */
void GameFlag_SetNibble(s32 flag, s32 value)
{
    s32 field_mask = 0xF;
    s32 shift = 4 & flag;
    s32 mask = field_mask << shift;
    u8 *bytes = (u8 *)GameFlagBytes;

    flag = ((u32)flag << 0x14) >> 0x17;
    bytes[flag] = (u8)((bytes[flag] & ~mask) |
                       ((value & field_mask) << shift));
}
