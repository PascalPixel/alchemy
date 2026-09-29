/* resource_39e:0200a7ec..0200aad0 (740 bytes with pool), still linked from
 * the listing. Remaining difference: its messages have catalogue names now; 170 halfwords
 * still differ from the ROM, and it names symbols no link defines
 * (bump_step). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgShianCanReadMindsKnowCurious[];
extern u8 MsgShianExcellentRobin[];
extern u8 MsgShianMonstersWaitInHidingWould[];
extern u8 MsgShianDontTryHard[];
/* Declarations and helpers: games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/TEMPLE.H. */

void FieldScene_RunScene39e_020027ec(void)
{
    u32 i;
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
    *(u8 *)(((s32)Engine_ActorGet(8)) + 91) = 0;
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
    L_0200299c:
        if (Event_ChooseYesNo(0, 0) == 1) {
            Event_Wait(10);
            Actor_ShowEmote(8, 0x102, 60);
            Event_SetMessage((s32)MsgShianDontTryHard);
            Event_OpenMessage(8, 0);
            goto L_0200299c;
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
