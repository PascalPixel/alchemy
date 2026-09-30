/*
 * Draft: GameFlag_SetBit, GameFlag_ClearBit and GameFlag_Toggle as in ☀️; ⚓️
 * loads the flag bytes between masking the bit number and shifting the mask,
 * one slot earlier than this compiles. Links as recon/tla/raw/08016bdc.s.
 */
#include "TYPES.H"

extern u8 GameFlagBytes[];

s32 GameFlag_SetBit(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;

    flag_mask = 1 << (7 & flag);
    flag_bytes = (u8 *)GameFlagBytes;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = (u8)(flag_bytes[flag] | flag_mask);
    return flag;
}

void GameFlag_ClearBit(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;

    flag_mask = ~(1 << (7 & flag));
    flag_bytes = (u8 *)GameFlagBytes;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = (u8)(flag_bytes[flag] & flag_mask);
}

u32 GameFlag_Toggle(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;
    s32 flag_value;

    flag_mask = 1 << (7 & flag);
    flag_bytes = (u8 *)GameFlagBytes;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = (u8)(flag_bytes[flag] ^ flag_mask);
    flag_value = flag_bytes[flag] & flag_mask;
    return (u32)((0 - flag_value) | flag_value) >> 31;
}
