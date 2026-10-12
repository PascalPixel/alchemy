/* Near miss, differ 19 of 40: the native rebuilds the scale constant inside
 * the loop in r2 and copies it to r1; through the inline layer the draft
 * builds it twice, and as a direct call or a scale local it hoists it into
 * r6 (differ 26). for/while forms were tried. Func_020039a8 is the same routine for actors 14 and 16. */
#include "../../../../games/tla/src/field/overlay_653/local.h"

void Func_0200393c(void)
{
    struct ObjectRuntime *object = Object_GetById(13);

    Func_020051c0();
    Func_02005370(0);
    do {
        object->y -= 0x40000;
        Func_02000cf8_(15, 0x4ccc, 0x4ccc);
        WaitFrames(1);
    } while (object->y > -0x140000);
    Func_02005230(15, 0, 0);
    ((struct EventWork1ac *)Ram_HeapSlots->event_work)->value = 0x209;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020052f8(6);
}

