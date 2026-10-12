#include "../../../../games/tla/src/field/overlay_66d/local.h"

static __inline__ void Func_02000534_(const void *script, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000534(script, x, z);
}

void Func_02000368(void)
{
    u8 *work = Ram_HeapSlots->event_work;
    s32 index;
    u32 id;

    Func_0200054c();
    Func_020005c4(0);
    for (id = 8; id <= 63; id++) {
        struct ObjectRuntime *object = Object_GetById(id);

        if (object != NULL)
            object->motion_flags = 0;
    }
    work += 0x154;
    index = *(s16 *)work;
    Func_020005f4(158);
    index -= 4;
    Func_02000534_(Data_0200064c[index].script, Data_0200064c[index].x, Data_0200064c[index].z);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    Object_GetById(gPartyState.current_owner)->motion_flags = 0;
    Object_SetModeById(gPartyState.current_owner, 2);
    Func_020005a4(*(s16 *)work);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000554();
}
