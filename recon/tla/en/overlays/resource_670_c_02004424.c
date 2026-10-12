/* Draft, differ 2 of 95: the native starts the effect value before it loads the event work
 * (mov r10, r3; movs r6, #129; ldr r3, [r3, #108]; movs r2, #214); with the empty asm below the
 * load comes first. Measured: without a barrier both constants start before the load (differ 4);
 * a one-pass block is the same 2; the value split around a "+l" barrier (129, then doubled plus
 * 255) gives the native order but swaps the callback and the value in r5 and r6 (differ 16); a
 * barrier holding the heap slots' address is far off (differ 38). The scene layer's inlines and
 * the task-add temporary are needed for the rest (differ 68 as direct calls). */
#include "../../../../games/tla/src/field/overlay_670/ov670.h"

void Func_020032b0(void);
void Func_02005bd4(s32 a, s32 b);
void Func_02005bdc(s32 a, s32 b);

void Func_02004424(void)
{
    struct Ov670EventWork *work;

    Func_02005a9c();
    Func_02005c3c(0);
    Object_SetPartAttribute(Object_GetById(gPartyState.current_owner), 15);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
    Func_020032b0();
    OV670_TASK_ADD(Func_02002ccc, 1152);
    work = Ram_HeapSlots->event_work;
    /* FAKEMATCH: the native loads the event work before it builds the effect's offset (differ 4 without the barrier). */
    asm volatile("");
    work->screen_effect = 513;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Battle_WaitMode0(40);
    Func_02005b4c((s32)MsgOhTheMagic);
    Ov670_MesPut(17, 0);
    Ov670_ObjTurn(17, 0x3000);
    Ov670_MesPut(0x4011, 0);
    Ov670_ObjTurn(17, 0x5000);
    Ov670_MesPut(0x4011, 0);
    Scheduler_RemoveCallbackFar(Func_02002ccc);
    Ov670_ObjTurn(17, 0xd000);
    Func_02005bdc(0x10000, 0);
    Func_02005bd4(0, 0);
    ((struct Ov670EventWork *)Ram_HeapSlots->event_work)->screen_effect = 513;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    GameFlag_SetBit(0x8fc);
    GameFlag_SetBit(0x109);
    Func_02005bc4(12);
}
