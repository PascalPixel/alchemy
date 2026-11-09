#include "lemurian_ship.h"
#include "event_inline.h"

extern const u8 gData_02002de8[];

void Func_02000438(void) {
  if (GameFlag_Test(0x982)) {
    struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
    struct MainRoutineWork *party = &gMainRoutineWork;
    struct LemurianShipObject *leader;
    s32 x;
    s32 cell;

    
    leader = ((struct LemurianShipObject *)Actor_GetById(party->currentOwner));
    Func_02002c68();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Actor_SetModeById(party->currentOwner, 2);
    Actor_SetSpeedParameters(party->currentOwner, 0x8000, 0x4000);
    x = leader->x;
    cell = x >> 16;
    Actor_SetMoveTarget(leader, ((8 - cell % 16) << 16) + x, leader->y, leader->z);
    Script_WaitForEventTimeout(leader);
    leader->facing = 0xc000;
    Actor_SetMovementTypeById(party->currentOwner,
                                            gData_02002de8);
    wait(12);
    Sound_Play(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(work->exit);
  }
}
