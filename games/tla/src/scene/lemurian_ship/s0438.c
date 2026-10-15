#include "lemurian_ship.h"

extern const u8 Data_02002de8[];

void Func_02000438(void)
{
    if (GameFlag_Test(0x982)) {
        struct LemurianShipEventWork *work = Ram_HeapSlots->event_work;
        struct PartyState *party = &gPartyState;
        struct LemurianShipObject *leader;
        s32 x;
        s32 cell;

        do { } while (0); /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        leader = Object_GetById(party->current_owner);
        Func_02002c68();
        Func_02002da8(0);
        LemurianShip_CamMove(-1, -1, -1, 0);
        Object_SetModeById(party->current_owner, 2);
        ObjectMotion_SetSpeedParameters(party->current_owner, 0x8000, 0x4000);
        x = leader->x;
        cell = x >> 16;
        Func_02002c18(leader, ((8 - cell % 16) << 16) + x, leader->y, leader->z);
        Func_02002c20(leader);
        leader->facing = 0xc000;
        ObjectMotion_EnableActionAndSetCallback(party->current_owner, Data_02002de8);
        WaitFrames(12);
        Func_02002de0(123);
        Event_FinishScreenTransition();
        Event_WaitTransitionFrames();
        Func_02002d70(work->exit);
    }
}
