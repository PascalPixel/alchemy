#include "local.h"

void Func_02000cd8(s32 id, s32 dy, s32 scale);
extern s32 Data_020062a0;
extern s32 Data_020062a4;

// The leader and actor 13 rise into view while actor 15 scales.
void Func_02003684(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    struct ObjectRuntime *first = Object_GetById(13);
    /* FAKEMATCH: the second actor pinned to r6 leaves the leader in r8 as the native has; unpinned, the two trade registers (differ 24). */
    register struct ObjectRuntime *second asm("r6") = Object_GetById(15);

    Func_020051c0();
    Func_02005370(0);
    Func_02005388(162);
    Motion_CamBounds_(-1, -1, -1, 0);
    WaitFrames_(1);
    leader->motionFlags = 0;
    Object_SetPositionAndResetMotion_(Object_GetById(gPartyState.currentOwner), 0x1b80000, -0x140000, 0xc80000);
    ObjectDispatch_SetSingleChildFlags_(Object_GetById(gPartyState.currentOwner), 0);
    Object_SetPositionAndResetMotion_(Object_GetById_(13), 0x1b80000, -0x140000, 0xcc0000);
    Object_SetPositionAndResetMotion_(Object_GetById_(15), 0x1b80000, -0x140000, 0xc80000);
    ObjectMotion_SetActionVariant_(13, 2);
    ObjectMotion_SetActionVariant_(15, 3);
    {
        u8 *flags = &second->priorityFlags;
        s32 c = 2 | *flags;

        *flags = c;
    }
    second->scaleY = 0x400;
    Object_SetModeById_(13, 4);
    Object_SetModeById_(15, 2);
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    // A goto loop, as in Func_0200393c.
rise:
    first->y += 0x40000;
    leader->y += 0x40000;
    Func_02000cd8(15, 0x4ccc, 0x4ccc);
    WaitFrames_(1);
    /* FAKEMATCH: as in Func_0200393c, a volatile read for the test. */
    if (*(volatile s32 *)&first->y < 0)
        goto rise;
    Func_02005178_(0, 0, 1, 1, 27, 12);
    Data_020062a0 = 2000;
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 516;
    Func_020051c8();
}

// As Func_02003684, for actors 14 and 16 at another place.
void Func_020037e0(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    struct ObjectRuntime *first = Object_GetById(14);
    /* FAKEMATCH: as in Func_02003684, the second actor pinned to r6. */
    register struct ObjectRuntime *second asm("r6") = Object_GetById(16);

    Func_020051c0();
    Func_02005370(0);
    Func_02005388(162);
    Motion_CamBounds_(-1, -1, -1, 0);
    WaitFrames_(1);
    leader->motionFlags = 0;
    Object_SetPositionAndResetMotion_(Object_GetById(gPartyState.currentOwner), 0x2580000, -0x140000, 0xe80000);
    ObjectDispatch_SetSingleChildFlags_(Object_GetById(gPartyState.currentOwner), 0);
    Object_SetPositionAndResetMotion_(Object_GetById_(14), 0x2580000, -0x140000, 0xec0000);
    Object_SetPositionAndResetMotion_(Object_GetById_(16), 0x2580000, -0x140000, 0xe80000);
    ObjectMotion_SetActionVariant_(14, 2);
    ObjectMotion_SetActionVariant_(16, 3);
    {
        u8 *flags = &second->priorityFlags;
        s32 c = 2 | *flags;

        *flags = c;
    }
    second->scaleY = 0x400;
    Object_SetModeById_(14, 4);
    Object_SetModeById_(16, 2);
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    // A goto loop, as in Func_0200393c.
rise:
    first->y += 0x40000;
    leader->y += 0x40000;
    Func_02000cd8(16, 0x4ccc, 0x4ccc);
    WaitFrames_(1);
    /* FAKEMATCH: as in Func_0200393c, a volatile read for the test. */
    if (*(volatile s32 *)&first->y < 0)
        goto rise;
    Func_02005178_(0, 0, 1, 1, 37, 14);
    Data_020062a4 = 2000;
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 516;
    Func_020051c8();
}

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
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
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
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020052f8(7);
}
