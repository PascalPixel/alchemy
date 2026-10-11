#include "ov68f.h"

void Func_020007dc(s32 step)
{
    struct Ov68fPosition position;

    switch (step) {
    case 1:
        break;
    case 2:
        position.x = 0x15b0000;
        position.y = 0;
        position.z = 0x1780000;
        Func_02000708(&position, 1, 0xcccc);
        break;
    case 3:
        Func_02002fc0(31, 85, 20, 85, 3, 4);
        position.x = 0x15b0000;
        position.y = 0;
        position.z = 0x1780000;
        Func_02000708(&position, 8, 0x18000);
        break;
    }
}

void Func_02000840(struct ObjectRuntime *object)
{
    struct Ov68fEffectParams params;
    u32 phase = gFrameCount & 7;
    s32 dx;
    s32 dy;

    if (phase == 0) {
        {
            /* FAKEMATCH: the native builds the mask before the random draw. */
            s32 mask = 15;

            dx = mask & Random16Far();
        }
        dy = Random16Far() & 7;
        params.kind = 673;
        Func_020000b8(object->x + ((dx - 8) << 16), object->y + ((dy - 8) << 16), object->z, 0,
                      0x8000, phase, 0x100000, &params);
    }
}

void Func_020008a8(void)
{
    struct ObjectRuntime *object = Object_GetById(8);

    Func_02003048();
    Func_02003148(0);
    GameFlag_SetBit(0x944);
    Call6((void (*)())Func_02002fe8, 11, 22, 3, 2, 20, 22);
    object->gravity = 0x8000;
    object->motion_flags = 3;
    OverlayObject_WaitUntilIdle(object);
    Func_02003088(8, 0, 0);
    Func_02003088(68, 0x1580000, 0x1740000);
    Object_GetById(68)->update = (void (*)(void))Func_02000840;
    Func_02003050();
}

void Func_02000920(void)
{
    Func_02003048();
    Func_02003148(0);
    UiText_ShowPositionedMessageAndWait(MsgThereSSomething, 1);
    if (GameFlag_Test(0x949)) {
        struct EventWork *work = Ram_HeapSlots->event_work;

        work->unknown_15a = 1;
    }
    Func_02003050();
}
