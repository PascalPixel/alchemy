#include "lemurian_ship.h"

extern const u8 gData_02002de8[];

void Func_02000438(void) {
  if (GameFlag_Test(0x982)) {
    struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
    struct Adventure *party = &gAdventure;
    struct LemurianShipObject *leader;
    s32 x;
    s32 cell;

    /* FAKEMATCH: a one-pass block loads the adventure before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    leader = Object_GetById(party->currentOwner);
    Func_02002c68();
    Func_02002da8(0);
    LemurianShip_CamMove(-1, -1, -1, 0);
    Object_SetModeById(party->currentOwner, 2);
    ObjectMotion_SetSpeedParameters(party->currentOwner, 0x8000, 0x4000);
    x = leader->x;
    cell = x >> 16;
    Func_02002c18(leader, ((8 - cell % 16) << 16) + x, leader->y, leader->z);
    Func_02002c20(leader);
    leader->facing = 0xc000;
    ObjectMotion_EnableActionAndSetCallback(party->currentOwner,
                                            gData_02002de8);
    WaitFrames(12);
    Func_02002de0(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02002d70(work->exit);
  }
}
