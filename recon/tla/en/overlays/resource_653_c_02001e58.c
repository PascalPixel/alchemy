/* Near miss, differ 2 of 92 (one reordered pair): the native loads the party
 * state address (ldr r5) before the byte store of motion_flags; the draft
 * stores first. Both insns tie on priority in sched2 and the store has more
 * dependents. Tried: party pointer temporaries, one-pass blocks, a cast
 * store, the inline layer and value-returning callees. */
#include "../../../../games/tla/src/field/overlay_653/local.h"

void Func_02001e58(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    s32 x = leader->x >> 20;
    s32 z = leader->z >> 20;

    if (z == 5) {
        if (x == 9 && (gInput.held & 16) == 0)
            return;
        if (x == 11 && (gInput.held & 32) == 0)
            return;
    } else if (x == 10 && z == 6 && (gInput.held & 64) == 0) {
        return;
    }
    Func_020051c0();
    Func_02005370(0);
    leader->motion_flags = 0;
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset(gPartyState.current_owner, 168, 80);
    Func_020052b0(gPartyState.current_owner, 0xc000);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
    Object_SetModeById(gPartyState.current_owner, 13);
    Func_02005160(leader, 0xa80000, -0xc0000, 0x500000);
    Func_02005168(leader);
    Object_SetModeById(gPartyState.current_owner, 10);
    Func_020053d8(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020052f8(8);
}

