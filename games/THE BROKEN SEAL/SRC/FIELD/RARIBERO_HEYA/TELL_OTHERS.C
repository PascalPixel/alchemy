#include "HEYA.H"
extern u8 MsgRariberoTellOthers[];

/*
 * "But tell me, what of the others?" Actor 11 asks, and the party and actor
 * 12 answer in turn, each beat an action on one actor and then a wait; the
 * last line stays open for the question that follows.
 */
void FieldScene_RunSecondaryScript(void)
{
    Event_SetMessage((s32)MsgRariberoTellOthers);
    Event_Wait(20);

    Actor_RunRepeatedMotion(11, 2);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(10);

    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 50);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);

    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);

    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);

    Actor_RunRepeatedMotion(12, 2);
    Event_Wait(20);
    Event_ShowMessage(12, 0);
    Event_Wait(20);

    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Event_Wait(25);

    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(30);

    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);

    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);

    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);

    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
    Event_OpenMessage(0x2002, 0);
}
