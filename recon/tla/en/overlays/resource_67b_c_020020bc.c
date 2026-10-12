// Near miss, differ 2: the listing sets the mode (movs r1, #2) before it loads the owner (ldr r0, [r5]);
// here the load comes first. A value-returning call is the same; the mode behind a barrier loses the shared
// owner address and zero (differ 34).
#include "../../../../games/tla/src/field/overlay_67b/local.h"

void Func_02002998(void);
void Func_020029a0(void);
void Func_02002a48(s32 a);
void Func_02002a70(s32 cue);
void Func_02002a18(s32 a);
s32 Func_02002030(struct ObjectRuntime *object);

void Func_020020bc(s32 arg0)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);

    Func_02002998();
    Func_02002a48(0);
    Func_02002a70(228);
    object->update = (void (*)(void))Func_02002030;
    object->motion_flags = 0;
    object->speed_limit = 0x3333;
    ((s32 (*)(u32, s32))Object_SetModeById)(gPartyState.current_owner, 2);
    ObjectMotion_OffsetPositionAndResetMotion(gPartyState.current_owner, 0, -8);
    ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.current_owner);
    Object_SetPartAttribute(Object_GetById(gPartyState.current_owner), 9);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
    object->update = 0;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02002a18(arg0);
    Func_020029a0();
}
