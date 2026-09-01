#include "types.h"

#define BattleEffect_PositionSprite Func_080a1a40

extern u8 Data_080af294[];
extern u8 Data_080af29d[];
extern volatile u32 Data_03001e40;
extern u8 *Data_03001f2c;

void BattleEffect_PositionSprite(s32 x_offset, s32 y_offset)
{
    u8 *state;
    u8 *anchor;
    u8 *sprite;
    s32 x;
    s32 y;
    s32 phase_index;

    state = Data_03001f2c;
    phase_index = (Data_03001e40 >> 1) & 7;
    sprite = *(u8 **)(state + 20);
    x = Data_080af294[phase_index];
    anchor = *(u8 **)(state + 16);
    x += x_offset;
    x += *(u16 *)(anchor + 12) * 8;
    x += 8;
    {
        s32 attr;

        *(u16 *)(sprite + 6) = x;
        x &= 0xffff;
        attr = *(u16 *)(sprite + 22);
        x &= 0x01ff;
        attr &= -0x200;
        attr |= x;
        *(u16 *)(sprite + 22) = attr;
    }
    phase_index = (Data_03001e40 >> 1) & 7;
    y = Data_080af29d[phase_index];
    y += y_offset;
    y += *(u16 *)(anchor + 14) * 8;
    y += 8;
    *(u16 *)(sprite + 8) = y;
    y &= 0xffff;
    *(u8 *)(sprite + 20) = y;
}
