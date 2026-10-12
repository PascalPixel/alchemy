#include "local.h"

void Func_02000d84(void)
{
    struct Ov695EventWork *work = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object;
    u32 id;

    Func_02001080();
    Func_020011e0(0);
    id = 8;
next:
    object = Object_GetById(id);
    if (object)
        object->motion_flags = 0;
    id++;
    if (id < 64)
        goto next;
    Object_GetById(gPartyState.current_owner)->motion_flags = 0;
    {
        s32 mode = 2;

        /* FAKEMATCH: the barrier holds r0 until the mode is set, so the leader is loaded after it as the native has; plain, or with the mode pinned to r1, the leader is loaded first (one reordered row). */
        __asm__("" : "+l"(mode) : : "r0");
        Object_SetModeById(gPartyState.current_owner, mode);
    }
    ObjectMotion_CenterOnColumn(gPartyState.current_owner, 2, -8);
    Battle_WaitMode0(5);
    Func_02001210(123);
    Func_02001198(work->value_154);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02001088();
}
