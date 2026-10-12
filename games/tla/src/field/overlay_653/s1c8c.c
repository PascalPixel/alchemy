#include "local.h"

struct MapCell {
    u8 unknown_0[2];
    u8 unknown_2;
    u8 unknown_3;
};

struct MapWork170 {
    u8 unknown_000[0x170];
    struct MapCell *cells;
};

void Func_02001c8c(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    s32 priority = ((struct FieldSprite *)leader->animation)->priority;

    ObjectMotion_SetActionVariant(13, priority);
    ObjectMotion_SetActionVariant(14, priority);
    ObjectMotion_SetActionVariant(15, priority);
}

void Func_02001cc4(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    struct MapWork170 *map = (struct MapWork170 *)Ram_HeapSlots->map_work;

    struct MapCell *cell = &map->cells[Func_020053b0()];

    if (cell->unknown_2 != 0) {
        leader->y = 0;
        leader->terrain_height = 0;
    }
}

void Func_02001d00(void)
{
    Func_020051c0();
    Func_02005370(0);
    UiText_ShowPositionedMessageAndWait((s32)&MsgScene653No10, 1);
    Func_020051c8();
}

void Func_02001d20(void)
{
    Func_020051c0();
    Func_02005370(0);
    UiText_ShowPositionedMessageAndWait((s32)&MsgScene653No01, 1);
    Func_020051c8();
}

void Func_02001d40(void)
{
    Func_020051c0();
    Func_02005370(0);
    ObjectMotion_EnableActionAndResetMotion(9);
    WaitFrames(1);
    ObjectMotion_SetAngleToward(9, gPartyState.current_owner, 0);
    Func_02005280((s32)&MsgScene653No02);
    Func_020052a0(9, 0);
    Object_GetById(9)->angle = 0x3000;
    WaitFrames(1);
    ObjectMotion_EnableActionAndSetCallback(9, Data_02005514);
    WaitFrames(1);
    Func_020051c8();
    GameFlag_SetBit(0x845);
}

void Func_02001db4(void)
{
    Func_020051c0();
    Func_02005370(0);
    ObjectMotion_SetAngleToward(10, gPartyState.current_owner, 0);
    Func_02005280((s32)&MsgScene653No03);
    Func_02005298(10, 0);
    Object_GetById(10)->angle = 0x5000;
    WaitFrames(1);
    Func_020051c8();
    GameFlag_SetBit(0x846);
}

void Func_02001e0c(void)
{
    Func_020051c0();
    Func_02005370(0);
    Func_02005280((s32)&MsgScene653No09);
    Func_020052a0(13, 0);
    Func_020051c8();
}

void Func_02001e30(void)
{
    Func_020052c8(9, 0x100, 0);
    Func_02001d40();
}

void Func_02001e44(void)
{
    Func_020052c8(10, 0x100, 0);
    Func_02001db4();
}

