/* Near miss: differ 2 of 54. Func_02002e94 called as value-returning sets
 * its constants before the owner as the native does (differ 4 with both
 * calls void). Left: the offset call. The native sets r1, then loads the owner
 * into r0, then sets r2. In sched2 all three tie at priority 75 and class 3;
 * rank_for_schedule then prefers more dependents, then LUID. Void, each has
 * four dependents and the owner goes first (LUID); value-returning, the
 * owner loses one (the call now sets r0) and r2 goes before it. The inline
 * layer, call.h's Call3 and Value3 give the same; a "+l" asm on the zero x
 * shares the stores' zero (differ 13). */
#include "../../../../games/tla/src/field/overlay_686/ov686.h"

void Func_020011c0(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);

    Func_02002e44();
    Func_02002edc(0);
    Object_SetModeById(gPartyState.current_owner, 0);
    Func_02002e9c(gPartyState.current_owner, 258);
    object->motion_flags = 0;
    object->action_flags = 0;
    ((s32 (*)(s32, s32, s32))ObjectMotion_OffsetPositionAndResetMotion)(gPartyState.current_owner, 0, 8);
    ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.current_owner);
    ((s32 (*)(s32, s32, s32))Func_02002e94)(gPartyState.current_owner, 256, 0);
    Battle_WaitMode0(10);
    Ov686_CamBounds(-1, -1, -1, 0);
    Func_02002eec(204);
    Func_02000b30(gPartyState.current_owner, 188);
    object->action_flags = 1;
    Func_02002e4c();
}
