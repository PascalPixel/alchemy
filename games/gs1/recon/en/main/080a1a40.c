#include "types.h"

#define BattleEffect_PositionSprite Func_080a1a40

void BattleEffect_PositionSprite(s32 x_offset, s32 y_offset)
{
    u8 *state = *(u8 **)0x03001f2c;
    u8 *anchor = *(u8 **)(state + 16);
    u8 *sprite = *(u8 **)(state + 20);
    u32 phase = (*(u32 *)0x03001e40 >> 1) & 7;
    u16 x = ((u8 *)0x080af294)[phase] + x_offset +
        (*(u16 *)(anchor + 12) * 8) + 8;
    u16 y = ((u8 *)0x080af29d)[phase] + y_offset +
        (*(u16 *)(anchor + 14) * 8) + 8;

    *(u16 *)(sprite + 6) = x;
    *(u16 *)(sprite + 22) =
        (*(u16 *)(sprite + 22) & 0xfe00) | (x & 0x01ff);
    *(u16 *)(sprite + 8) = y;
    *(u8 *)(sprite + 20) = y;
}
