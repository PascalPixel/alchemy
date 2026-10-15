#include "local.h"

// Actor 13 rises out of view while actor 15 scales, then the scene moves on.
void Func_0200393c(void)
{
    struct ObjectRuntime *object = Object_GetById(13);

    Func_020051c0();
    Func_02005370(0);
    // A goto loop: as a do-while, loop.c hoists the scale constant into r6 (differ 19).
rise:
    object->y -= 0x40000;
    Func_02000cf8(15, 0x4ccc, 0x4ccc);
    WaitFrames(1);
    /* FAKEMATCH: a volatile read for the test keeps GCSE from carrying y around the loop in r2; the native loads it again at the top (differ 13 plain). */
    if (*(volatile s32 *)&object->y > -0x140000)
        goto rise;
    Func_02005230(15, 0, 0);
    ((struct EventWork1ac *)Ram_HeapSlots->event_work)->value = 521;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020052f8(6);
}

// As Func_0200393c, for actors 14 and 16.
void Func_020039a8(void)
{
    struct ObjectRuntime *object = Object_GetById(14);

    Func_020051c0();
    Func_02005370(0);
    // A goto loop, as in Func_0200393c.
rise:
    object->y -= 0x40000;
    Func_02000cf8(16, 0x4ccc, 0x4ccc);
    WaitFrames(1);
    /* FAKEMATCH: as in Func_0200393c, a volatile read for the test (differ 13 plain). */
    if (*(volatile s32 *)&object->y > -0x140000)
        goto rise;
    Func_02005230(16, 0, 0);
    ((struct EventWork1ac *)Ram_HeapSlots->event_work)->value = 521;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020052f8(7);
}
