// near miss: differ 4 (listing 76 instructions, draft 76); scheduling at entry: the native loads the event work between the owner's address and its offset
#include "../../../../games/tla/src/field/overlay_670/ov670.h"

void Func_02003a08(s32 x, s32 z)
{
    struct PartyState *party = &gPartyState;
    struct Ov670EventWork *work = Ram_HeapSlots->event_work;
    struct Ov670Object *leader = Object_GetById(party->current_owner);

    Func_02005a9c();
    Func_02005c3c(0);
    leader->motion_flags = 0;
    Ov670_ObjSpeed(gPartyState.current_owner, 0x10000, 0x8000);
    Ov670_ObjWalk(gPartyState.current_owner, x, z);
    Ov670_ObjTurn(gPartyState.current_owner, 0xc000);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
    Object_SetModeById(gPartyState.current_owner, 13);
    Func_02005a54(leader, x << 16, 0xfff40000, z << 16);
    Func_02005a5c(leader);
    Object_SetModeById(gPartyState.current_owner, 10);
    Func_02005c9c(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02005bc4(work->exit);
}
