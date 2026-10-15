// near miss: differ 4 (listing 76 instructions, draft 76); scheduling at entry: the native loads the event work between the owner's address and its offset
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

void Func_02003a08(s32 x, s32 z)
{
    struct PartyState *party = &gPartyState;
    struct KibomboEventWork *work = Ram_HeapSlots->eventWork;
    struct KibomboObject *leader = Object_GetById(party->currentOwner);

    Func_02005a9c();
    Func_02005c3c(0);
    leader->motionFlags = 0;
    Kibombo_ObjSpeed(gPartyState.currentOwner, 0x10000, 0x8000);
    Kibombo_ObjWalk(gPartyState.currentOwner, x, z);
    Kibombo_ObjTurn(gPartyState.currentOwner, 0xc000);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner), 0);
    Object_SetModeById(gPartyState.currentOwner, 13);
    Func_02005a54(leader, x << 16, 0xfff40000, z << 16);
    Func_02005a5c(leader);
    Object_SetModeById(gPartyState.currentOwner, 10);
    Func_02005c9c(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02005bc4(work->exit);
}
