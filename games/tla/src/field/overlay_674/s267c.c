#include "ov674.h"

void Func_0200267c(void)
{
    struct Ov674EventWork *work = Ram_HeapSlots->event_work;
    struct PartyState *party = &gPartyState;
    struct ObjectRuntime *object;
    s16 *exit;
    s32 index;
    s32 cell;

    asm volatile("" : "+l"(party)); /* FAKEMATCH: the listing loads the party state address before it builds the owner offset */
    object = Object_GetById(party->current_owner);
    Func_02003e64();
    Func_02003fa4(0);
    Call4((void (*)())Motion_CamBounds, -1, -1, -1, 0);
    Object_SetModeById(party->current_owner, 2);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, party->current_owner, 0x8000, 0x4000);
    cell = object->x >> 16;
    Func_02003e0c(object, ((8 - cell % 16) << 16) + object->x, object->y, object->z);
    Func_02003e14(object);
    object->angle = 0xc000;
    exit = &work->exit;
    index = *exit;
    asm volatile("" : "+l"(index)); /* FAKEMATCH: the listing loads the exit before it builds the cue number */
    Call1((void (*)())Func_02003ffc, 158);
    asm volatile("" : "+l"(index)); /* FAKEMATCH: the listing subtracts one after the call, not before it */
    index--;
    Call3((void (*)())Func_02003e1c, Data_020046bc[index].value, Data_020046bc[index].pair.first, Data_020046bc[index].pair.second);
    ObjectMotion_EnableActionAndSetCallback(party->current_owner, Data_02004028);
    WaitFrames(12);
    Func_02003ffc(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02003f7c(*exit);
}
