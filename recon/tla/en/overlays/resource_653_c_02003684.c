/* Draft, differ 28 of 141, written from the listing: the loop keeps its
 * constants inside through one step variable set twice (plain, loop.c hoists
 * 0x40000 and 0x4ccc: differ 42). Unpinned, the leader takes r6 and the
 * second object r8, the native the other way round (differ 33); with the
 * leader pinned to r8, left: the owner offset is built in r2 where the native
 * uses r1 (one register), the 15 and 2 of the second mode call swap, and the
 * loop loads first->y into r1 and reloads the leader through r3. */
#include "../../../../games/tla/src/field/overlay_653/local.h"

void Func_02000cd8(s32 id, s32 dy, s32 scale);
extern s32 Data_020062a0;

void Func_02003684(void)
{
    register struct ObjectRuntime *leader asm("r8") = Object_GetById(gPartyState.current_owner);
    struct ObjectRuntime *first = Object_GetById(13);
    struct ObjectRuntime *second = Object_GetById(15);
    s32 step;

    Func_020051c0();
    Func_02005370(0);
    Func_02005388(162);
    Motion_CamBounds_(-1, -1, -1, 0);
    WaitFrames_(1);
    leader->motion_flags = 0;
    Object_SetPositionAndResetMotion_(Object_GetById(gPartyState.current_owner), 0x1b80000, -0x140000, 0xc80000);
    ObjectDispatch_SetSingleChildFlags_(Object_GetById(gPartyState.current_owner), 0);
    Object_SetPositionAndResetMotion_(Object_GetById_(13), 0x1b80000, -0x140000, 0xcc0000);
    Object_SetPositionAndResetMotion_(Object_GetById_(15), 0x1b80000, -0x140000, 0xc80000);
    ObjectMotion_SetActionVariant_(13, 2);
    ObjectMotion_SetActionVariant_(15, 3);
    second->priority_flags |= 2;
    second->scale_y = 0x400;
    Object_SetModeById_(13, 4);
    Object_SetModeById_(15, 2);
    ((struct EventWork1ac *)Ram_HeapSlots->event_work)->value = 521;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    do {
        step = 0x40000;
        first->y += step;
        leader->y += step;
        step = 0x4ccc;
        Func_02000cd8(15, step, step);
        WaitFrames_(1);
    } while (first->y < 0);
    Func_02005178_(0, 0, 1, 1, 27, 12);
    Data_020062a0 = 2000;
    ((struct EventWork1ac *)Ram_HeapSlots->event_work)->value = 516;
    Func_020051c8();
}
