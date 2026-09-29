/* The party walks in and actor 8, who can read minds, asks whether it is
 * curious. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TEMPLE.H"
extern u8 MsgShianCanReadMindsKnowCurious[];
extern u8 MsgShianExcellentRobin[];
extern u8 MsgShianMonstersWaitInHidingWould[];
extern u8 MsgShianDontTryHard[];

void ShianJiin_AskIfCurious(void)
{
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 168, 0x1f8);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_Wait(20);
    Actor_StartRepeatedMotion(8, 2);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    ((u8 *)Engine_ActorGet(8))[91] = 0;
    Audio_PlayCue(152);
    record = Actor_Get(8);
    *(s32 *)(record + 40) = 0x80000;
    Actor_SetAnimation(8, 1);
    Event_SetMessage((s32)MsgShianExcellentRobin);
    Event_ShowMessageAndWait(8, 0, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_SetAnimationAndWait(8, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(8, 0, 20);
        bump_step(2);
    } else {
        Event_Wait(10);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        bump_step(1);
        Event_ShowMessageAndWait(8, 0, 20);
        Actor_SetAnimationAndWait(8, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(8, 0, 20);
    }
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        Event_Wait(10);
        Actor_ShowEmote(8, 0x102, 60);
        Event_SetMessage((s32)MsgShianCanReadMindsKnowCurious);
        Event_OpenMessage(8, 0);
        /* FAKEMATCH: the question repeats through this label rather than a
         * while loop, which keeps the reference's block order. */
    ask_again:
        if (Event_ChooseYesNo(0, 0) == 1) {
            Event_Wait(10);
            Actor_ShowEmote(8, 0x102, 60);
            Event_SetMessage((s32)MsgShianDontTryHard);
            Event_OpenMessage(8, 0);
            goto ask_again;
        }
    }
    Event_SetMessage((s32)MsgShianMonstersWaitInHidingWould);
    Event_Wait(10);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(8, 0, 20);
        bump_step(1);
    } else {
        Event_Wait(10);
        Actor_RunRepeatedMotion(8, 2);
        bump_step(1);
        Event_ShowMessageAndWait(8, 0, 20);
    }
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_SetAnimation(8, 5);
    GameFlag_Set(0x893);
    Event_End();
}
