#include "TASK.H"

void RunPartyCountInteractionCopyA(s32 actorId)
{
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = (PartyInteractionRecord *)Engine_ActorGet(actorId);
    x = record->x;
    y = record->y;
    Event_Begin();

    if (Party_CountActiveOwners() <= 1) {
        Event_SetMessage(MSG_ROBIN_DID_GET_GOOD_LOOK);
        if (Event_AskYesNo(actorId, 0) == 0) {
            InitializeActorZero();
            InitializeSelectedActor(actorId);
            Actor_WalkTo(actorId, x, y + 0x40);
            Event_Wait(15);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, x, y);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, x, y + 0x20);
            Event_CloseScreen();
            Event_WaitForScreen();
            Event_RequestExit(11);
        }
    } else {
        Event_SetMessage(MSG_WAIT_SHOULDNT_DECIDE_WHERE_BEST);
        Event_ShowMessage(actorId, 0);
    }

    Event_End();
}

/*
 * Six steps, each clearing one game-state byte -- 896 through 936, eight
 * apart.  It reads no incoming argument, so it takes none.
 */
void FieldScene_RunSixSteps896To936(void)
{
    GameFlag_SetByte(896, 0);
    GameFlag_SetByte(904, 0);
    GameFlag_SetByte(912, 0);
    GameFlag_SetByte(920, 0);
    GameFlag_SetByte(928, 0);
    GameFlag_SetByte(936, 0);
}
