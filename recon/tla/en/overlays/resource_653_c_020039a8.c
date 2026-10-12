/* Near miss, differ 19 of 40: as Func_0200393c (the native builds the scale
 * constant once inside the loop and copies it). */
#include "../../../../games/tla/src/field/overlay_653/local.h"

void Func_020039a8(void)
{
    struct ObjectRuntime *object = Object_GetById(14);

    Func_020051c0();
    Func_02005370(0);
    do {
        object->y -= 0x40000;
        Func_02000cf8_(16, 0x4ccc, 0x4ccc);
        WaitFrames(1);
    } while (object->y > -0x140000);
    Func_02005230(16, 0, 0);
    ((struct EventWork1ac *)Ram_HeapSlots->event_work)->value = 0x209;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020052f8(7);
}

