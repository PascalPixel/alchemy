#include "types.h"
extern u8 GameFlagBytes[];

s32 GameFlag_Test(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;
    s32 flag_value;

    flag_mask = 1 << (7 & flag);
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes = (u8 *)GameFlagBytes;
    flag_value = flag_bytes[flag] & flag_mask;
    return (u32)((0 - flag_value) | flag_value) >> 0x1F;
}

s32 GameFlag_SetBit(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;

    flag_mask = 1 << (7 & flag);
    flag_bytes = (u8 *)GameFlagBytes;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = flag_bytes[flag] | flag_mask;
    return flag;
}

void GameFlag_ClearBit(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;

    flag_mask = ~(1 << (7 & flag));
    flag_bytes = (u8 *)GameFlagBytes;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = flag_bytes[flag] & flag_mask;
}

extern u8 GameFlagBytes[];

u32 GameFlag_Toggle(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;
    s32 flag_value;

    flag_mask = 1 << (7 & flag);
    flag_bytes = (u8 *)GameFlagBytes;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = flag_bytes[flag] ^ flag_mask;
    flag_value = flag_bytes[flag] & flag_mask;
    return (u32)((0 - flag_value) | flag_value) >> 31;
}

u8 GameFlag_GetByte(s32 flag)
{
    u32 byte_index;

    byte_index = flag << 20;
    flag = byte_index >> 23;
    return GameFlagBytes[flag];
}

u32 GameFlag_SetByte(u32 flag, u8 value)
{
    u32 shifted = flag << 20;
    flag = shifted >> 23;
    GameFlagBytes[flag] = value;
    return flag;
}

u8 GameFlag_IncrementByte(s32 flag)
{
    u8 *flag_bytes;

    flag_bytes = (u8 *)GameFlagBytes;
    flag = ((u32)flag << 0x14) >> 0x17;
    if (flag_bytes[flag] <= 0xFE) {
        flag_bytes[flag] = flag_bytes[flag] + 1;
    }
    return flag_bytes[flag];
}

u8 GameFlag_DecrementByte(s32 flag)
{
    u8 *flag_bytes;

    flag_bytes = (u8 *)GameFlagBytes;
    flag = ((u32)flag << 0x14) >> 0x17;
    if (flag_bytes[flag] != 0) {
        flag_bytes[flag] = flag_bytes[flag] + 0xFF;
    }
    return flag_bytes[flag];
}

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

void GameFlag_SetNibble(s32 flag, s32 value)
{
    s32 field_mask = 0xF;
    s32 shift = 4 & flag;
    s32 mask = field_mask << shift;
    u8 *bytes = (u8 *)GameFlagBytes;

    flag = ((u32)flag << 0x14) >> 0x17;
    bytes[flag] = (bytes[flag] & ~mask) |
                       ((value & field_mask) << shift);
}
