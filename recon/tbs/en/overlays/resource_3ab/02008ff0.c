/* Draft of resource_3ab 0x02008ff0..0x0200918c (412 bytes with pool),
 * Guards_CatchParty; the listing keeps the rows. Remaining difference: its messages have catalogue names now; 1 halfword
 * still differs from the ROM. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/VILLAGE.H"
extern u8 MsgRunpaGuardsCatchParty[];

/*
 * The guards challenge the party they have spotted, then march it back
 * down the road and watch for it again.
 */
void Guards_CatchParty(void)
{
    struct FieldActor *leader;
    s32 warning;

    if (GameFlag_IsSet(FLAG_GATE_PARTY_CAUGHT) != 0) {
        return;
    }
    GameFlag_Set(FLAG_GATE_PARTY_CAUGHT);
    Event_Begin();
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
    Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    Actor_StartRepeatedMotion(ACTOR_RIGHT_GUARD, 1);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 2, 60);
    warning = (s32)MsgRunpaGuardsCatchParty;
    Event_SetMessage(warning + CATCH_LEFT_GUARD_CHALLENGES);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x20000, 0x10000);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_SHAKE_HEAD);
    Event_Wait(35);
    Event_SetMessage(warning + CATCH_RIGHT_GUARD_WONDERS);
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 3, 30);
    Event_SetMessage(warning + CATCH_LEFT_GUARD_REFUSES_ENTRY);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_NOD);
    Event_Wait(25);
    Event_SetMessage(warning + CATCH_RIGHT_GUARD_SENDS_PARTY_OFF);
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
    Actor_WalkTo(ACTOR_LEFT_GUARD, leader->x.part.pixel - 1, leader->z.part.pixel);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 216);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 200);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 200);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WaitForMove(ACTOR_RIGHT_GUARD);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(12);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 272);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 256);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 256);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WaitForMove(ACTOR_RIGHT_GUARD);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_End();
    Task_AddCallback(Guards_Watch, TASK_PRIORITY_SCENE);
}
