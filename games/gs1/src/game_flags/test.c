#include "scene.h"
#include "game_flags.h"

/* game_flags/test.c */
s32 GameFlag_Test(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;
    s32 flag_value;

    flag_mask = 1 << (7 & flag);
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes = (u8 *)0x02000040;
    flag_value = flag_bytes[flag] & flag_mask;
    return (s32)((u32)((0 - flag_value) | flag_value) >> 0x1F);
}

/* game_flags/set.c */
s32 GameFlag_SetBit(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;

    flag_mask = 1 << (7 & flag);
    flag_bytes = (u8 *)0x02000040;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = (u8)(flag_bytes[flag] | flag_mask);
    return flag;
}

/* game_flags/clear.c */
void GameFlag_ClearBit(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;

    flag_mask = ~(1 << (7 & flag));
    flag_bytes = (u8 *)0x02000040;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = (u8)(flag_bytes[flag] & flag_mask);
}

/* game_flags/toggle.c */
u32 GameFlag_Toggle(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;
    s32 flag_value;

    flag_mask = 1 << (7 & flag);
    flag_bytes = (u8 *)0x02000040;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = (u8)(flag_bytes[flag] ^ flag_mask);
    flag_value = flag_bytes[flag] & flag_mask;
    return (u32)((0 - flag_value) | flag_value) >> 31;
}

/* game_flags/get_byte.c */
u8 GameFlag_GetByte(s32 flag)
{
    u32 byte_index;

    byte_index = (u32)flag << 20;
    flag = (u32)byte_index >> 23;
    return GameFlagBytes[flag];
}

/* game_flags/set_byte.c */
u32 GameFlag_SetByte(u32 flag, u8 value)
{
    u32 shifted = flag << 20;
    flag = shifted >> 23;
    GameFlagBytes[flag] = value;
    return flag;
}

/* game_flags/increment_byte.c */
u8 GameFlag_IncrementByte(s32 flag)
{
    u8 *flag_bytes;

    flag_bytes = (u8 *)0x02000040;
    flag = ((u32)flag << 0x14) >> 0x17;
    if (flag_bytes[flag] <= 0xFE) {
        flag_bytes[flag] = flag_bytes[flag] + 1;
    }
    return flag_bytes[flag];
}

/* game_flags/decrement_byte.c */
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
