/* Lunpa fortress: the guards challenge the party ("Who are you!?"),
 * talk it over and send the party back out to the fortress's second scene
 * at entrance 31. */
#include "FORTRESS.H"
#include "SCENE_IDS.H"
extern u8 MsgRunpaWho3[];

void RunpaJo_RunGuardChallenge(void)
{
    s32 line;

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(12, 1);
    Actor_SetAnimation(13, 1);
    Actor_SetAnimation(14, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(12, 0x100, 0);
    Event_Wait(30);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    line = (s32)MsgRunpaWho3;
    Event_SetMessage(line);
    Event_ShowMessage(12, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 0);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Event_Wait(65);
    Actor_FaceDirection(13, 0x5000, 0);
    Actor_FaceDirection(14, 0xd000, 0);
    Event_SetMessage((line + 1));
    Event_ShowMessage(13, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_SetMessage((line + 2));
    Event_ShowMessage(14, 0);
    Event_SetMessage((line + 3));
    Event_ShowMessage(12, 0);
    Actor_RunRepeatedMotion(13, 1);
    Event_SetMessage((line + 4));
    Event_ShowMessage(13, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_SetMessage((line + 5));
    Event_ShowMessage(14, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(60);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Event_Wait(70);
    Actor_WalkTo(12, 0x2a0, 88);
    Actor_WaitForMove(12);
#if !defined(TBS_EDITION_JA)
    /* The localized sequence adds this turn after actor 12 is placed. */
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
#endif
    Actor_SetAnimationAndWait(12, 3);
    Event_Wait(30);
    Event_SetMessage((line + 6));
    Event_ShowMessage(12, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_RunpaJo2, 31);
    gGameState.unknown_1f8[0x22b - 0x1f8] = 3;
    BattleFx_SetWeightedResult(98, 3);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Event_End();
    GameFlag_Set(0x94a);
}
