/* Near miss, differ 4 of 112: the angles of both Func_02003a38 calls in a temporary are
 * built first, as the listing does (differ 12 as constants); left is the last flag byte,
 * where the listing loads WaitFrames' argument between the load and the OR. */
#include "../../../../games/tla/src/field/overlay_654/kabe.h"
#include "call.h"

void Func_020024e4(void)
{
    struct KabeObject *object = Object_GetById(13);
    s32 angle;

    Func_020039d8();
    Func_02003b08(0);
    Value3((s32 (*)())Func_02003aa8, gPartyState.current_owner, 0x100, 0);
    Func_02003ad0()->motion_flags = 0;
    Call4((void (*)())Motion_CamBounds, 0x1480000, 0x200000, 0x2280000, 1);
    Func_02003ac8();
    angle = 0xc000;
    Call4((void (*)())Func_02003a38, 14, 0x1480000, 0x2700000, angle);
    WaitFrames(1);
    Object_LinkObjectAndSetCallback(gPartyState.current_owner, 14);
    object->priority_flags &= ~2;
    ObjectMotion_SetActionVariant(14, 3);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 14, 0x19999, 0xcccc);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 14, 328, 568);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 14, 328, 496);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 14, 280, 432);
    ObjectMotion_EnableActionAndResetMotion(gPartyState.current_owner);
    WaitFrames(1);
    angle = 0x4000;
    Call4((void (*)())Func_02003a38, 14, 0x1180000, 0x1480000, angle);
    ObjectMotion_SetActionVariant(14, 2);
    Object_GetById(14)->priority_flags |= 1;
    object->priority_flags |= 2;
    WaitFrames(1);
    GameFlag_SetBit(0x876);
    GameFlag_SetBit(0x877);
    Func_020039e0();
}
