#include "KAREI.H"

extern u8 MsgKareiFinishedOutHereBoardShip[];
extern u8 MsgKareiWantGoAshore[];
extern u8 MsgKareiTicketsPlease[];

/* The ticket taker at the gangway, actor 12. Before boarding he answers only
 * a leader facing him from the pier: with the ticket (item 235) in the party
 * he takes it, steps aside and lets the party aboard. Once aboard he offers
 * to let the party ashore, and after that asks them to come back aboard. */
void SceneDialogue_RunActor12Event(void)
{
    s32 owner;
    s32 slot;
    u8 *record;
    s16 angle;

    record = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    angle = (u16)((*(u16 *)(record + 6) + 0x2000) & ~0x3fff);
    Event_Begin();
    if (GameFlag_IsSet(0x8a7) != 0) {
        if (GameFlag_IsSet(0x8a9) != 0) {
            Event_SetMessage((s32)MsgKareiFinishedOutHereBoardShip);
            Event_OpenMessage(12, 0);
            goto done;
        }
        Event_SetMessage((s32)MsgKareiWantGoAshore);
        Event_OpenMessage(12, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage((s32)MsgKareiWantGoAshore + 1);
            Event_ShowMessage(12, 0);
            Actor_WalkToAndWait(12, 88, 0x508);
            Actor_FaceDirection(12, 0x4000, 0);
            Event_Wait(20);
            GameFlag_Set(0x8a9);
            goto done;
        }
        Event_SetMessage((s32)MsgKareiWantGoAshore + 2);
        Event_ShowMessage(12, 0);
    } else {
        if ((u16)angle != 0x8000) {
            return;
        }
        Event_SetMessage((s32)MsgKareiTicketsPlease);
        Event_ShowMessage(12, 0);
        if (GameFlag_IsSet(0x8a5) != 0) {
            owner = PartyInventory_FindOwner(235);
            slot = Inventory_Find(owner, 235);
            Actor_SetAnimationAndWait(12, 3);
            Actor_WalkToAndWait(12, 88, 0x508);
            Actor_FaceDirection(12, 0x4000, 0);
            bump_step(1);
            Event_ShowMessage(12, 0);
            Inventory_Discard(owner, slot);
            GameFlag_Set(0x8a7);
            record = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, *(s16 *)(record + 10), 0x518);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 72, 0x518);
            Actor_WalkToAndWait(12, 88, 0x518);
            Actor_FaceDirection(12, 0, 0);
        } else {
            Event_ShowMessage(12, 0);
        }
    }
done:
    Event_End();
}
