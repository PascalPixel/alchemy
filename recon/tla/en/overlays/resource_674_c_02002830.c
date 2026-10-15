/* Near miss, differ 8 of 157: at the first store to the event work's field 0x1ac the listing
 * shifts the offset, reloads the event work, then copies the offset to r11 and adds 85; this draft
 * copies it to r11 before the reload. A local for the work, a cast and plain pointer arithmetic give
 * the same code, and an empty loop note there changes the allocation (differ 34). */
#include "../../../../games/tla/src/scene/islets/islets.h"

void Func_02002830(void)
{
    struct IsletsEventWork *work;
    struct ObjectRuntime *owner = Object_GetById(gPartyState.current_owner);
    struct ObjectRuntime *object = Object_GetById(9);

    Func_02003eec(9, gPartyState.current_owner);
    Func_02003edc(10, 0x1110000, 0x2080000);
    WaitFrames(1);
    object->angle = 0xc000;
    owner->update = (void (*)(void))Func_02001278;
    Object_SetPartAttribute(Object_GetById(gPartyState.current_owner), 15);
    Object_SetModeById(9, 3);
    WaitFrames(1);
    work = Ram_HeapSlots->event_work;
    work->field_1ac = 0x201;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 9, 0x9999, 0x4ccc);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0x9999, 0x4ccc);
    Object_GetById(10)->update = (void (*)(void))Func_02000df8;
    object->update = (void (*)(void))Func_02000df8;
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 10, 280, 468);
    Call3((void (*)())ObjectMotion_SetPositionAndCommit, 9, 296, 452);
    Object_SetModeById(10, 1);
    owner->update = 0;
    object->update = 0;
    Object_GetById(10)->update = 0;
    Func_02003fac(gPartyState.current_owner, 9);
    Func_02003f24((s32)MsgHereWeAre);
    Func_02003f2c(9, 0);
    Object_SetModeById(9, 1);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x19999, 0xcccc);
    Object_SetPartAttribute(Object_GetById(gPartyState.current_owner), 0);
    ObjectMotion_Launch(gPartyState.current_owner, 6, 0);
    ObjectMotion_SetPositionAndCommit(gPartyState.current_owner, 312, 440);
    Call2((void (*)())Func_02003f3c, gPartyState.current_owner, 0x6000);
    Func_02003f2c(9, 0);
    Func_02003fb4();
    Call2((void (*)())Func_02003f3c, 9, 0x4000);
    work = Ram_HeapSlots->event_work;
    work->field_1ac = 0x100;
    Func_020034d8();
}
