#include "local.h"

void Func_020010d0(struct ObjectRuntime *object)
{
    s32 frames = 60;

    while (frames != 0) {
        WaitFrames(1);
        frames--;
        if (object->velocity_y == 0)
            break;
    }
}

void Func_020010ec(void)
{
    Func_02003d7c();
    Func_02003eac(0);
    UiText_ShowPositionedMessageAndWait(MsgCheckedThePsynergy, 1);
    Func_02003ef4(126);
    Func_02003eb4(999, 0);
    Battle_WaitMode0(10);
    GameFlag_ClearBit(0x142);
    UiText_ShowPositionedMessageAndWait(MsgCheckedThePsynergy + 1, 1);
    Func_02003d84();
}

void Func_02001138(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    Func_02003d1c(17, 39, 2, 1, 20, 39);
    Func_02003d1c(23, 39, 1, 1, object->x >> 20, 39);
}
