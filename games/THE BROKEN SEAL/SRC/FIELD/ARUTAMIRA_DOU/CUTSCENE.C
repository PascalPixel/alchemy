#include "ARUTAMIRA.H"
#include "CALL.H"
extern u8 MsgArutamiraDontTrustAnyone[];
extern u8 MsgArutamiraKiddingHaventActually[];
extern u8 MsgArutamiraSaidCouldntMove[];
extern u8 MsgArutamiraSee[];

/*
 * Every call site is written out separately and repeated calls must not be
 * folded: the sequence of distinct call words is what reproduces the
 * reference. The three record lookups near the end are null checked before
 * their stored coordinates are forwarded.
 */
void FieldScene_RunBranchingCutsceneSequence(void)
{
    u8 *record;
    s32 line;
    GameFlag_Set(0x960);
    Engine_AudioPlayCue(24);
    Event_Begin();
    Battle_ResetEffectCounter(); /* main:0808a460 */
    Event_SetMessage((s32)MsgArutamiraSee);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Camera_MoveTo(0xf80000, -1, 0xb80000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 192);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Call4(Motion_LaunchFromFocusedObject, 1, -16, 16, 0xc000);
    Call4(Motion_LaunchFromFocusedObject, 3, 0, 16, 0xc000);
    Value4(Motion_LaunchFromFocusedObject, 2, 16, 16, 0xc000);
    Engine_ActorWaitForMove(1);
    Engine_EventWait(20);
    Value3(Engine_ActorShowEmote, 2, 0x102, 0);
    Engine_EventWait(40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x100, 40);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Actor_SetAttachedEffect(8, 0x102); /* main:0808a1f0 */
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(20);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(2, 0);
    Event_Wait(10);
    Actor_ShowEmote(8, 0x100, 40);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x105, 40);
    Engine_EventShowMessage(8, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 30);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_ActorFaceDirection(1, 0xc000, 0);
    Event_Wait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x102, 40);
    Event_ShowMessage(8, 0);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(3, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(8, 0x106, 40);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Engine_ActorShowEmote(2, 0x101, 0);
    Event_Wait(60);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(8, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 40);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x102, 40);
    Event_OpenMessage(8, 0); /* main:0808a178 */
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Engine_EventSetMessage((s32)MsgArutamiraKiddingHaventActually);
        Engine_EventWait(20);
        Actor_ShowEmote(ACTOR_GERALD, 0x100, 40);
        Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
        Value3(Engine_ActorWalkByAndWait, 1, 0, -16);
        Engine_EventWait(10);
        Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 30);
        Engine_EventShowMessage(1, 0);
    } else {
        Engine_EventSetMessage((s32)MsgArutamiraDontTrustAnyone);
        Engine_EventWait(10);
        Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
        Value3(Engine_ActorWalkByAndWait, 1, 0, -16);
        Engine_EventWait(10);
        Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 30);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
    }

    line = (s32)MsgArutamiraSaidCouldntMove;
    Engine_EventSetMessage(line);
    Actor_ShowEmote(ACTOR_IVAN, 0x103, 40);
    Actor_SetSpeed(ACTOR_IVAN, 0x20000, 0x10000);
    Value3(Engine_ActorWalkByAndWait, 2, 0, -16);
    Engine_EventWait(10);
    Actor_FaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_MIA, 0);
    Engine_ActorFaceActor(2, 3, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Engine_ActorWalkByAndWait(1, 0, 16);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(1, 4);
    Engine_EventWait(30);
    Engine_ActorFaceActor(1, 0, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(30);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 30);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(30);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Engine_ActorWalkByAndWait(2, 0, 16);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    line += 7;
    Engine_EventSetMessage(line);
    Engine_EventWait(30);
    Actor_ShowEmote(8, 0x100, 40);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(2, 0);
    Event_Wait(10);
    Actor_ShowEmote(8, 0x108, 40);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 40);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Event_Wait(20);
    Actor_ShowEmote(8, 0x108, 50);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x102, 40);
    Event_ShowMessage(8, 0);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(3, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 40);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(30);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_MIA, 0);
    Engine_ActorFaceActor(2, 3, 0);
    Engine_EventWait(20);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Engine_AudioPlayCue(17);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Engine_ActorSetAnimation(1, 2);
    /* If the id-1 record lookup succeeds, forward its stored coordinates. */
    record = Engine_ActorGet(0);
    if (record != 0) {
        Engine_ActorSetDestination(1, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    Engine_ActorWaitForMove(1);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Engine_ActorSetAnimation(2, 2);
    /* If the id-2 record lookup succeeds, forward its stored coordinates. */
    record = Engine_ActorGet(0);
    if (record != 0) {
        Engine_ActorSetDestination(2, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    Engine_ActorWaitForMove(2);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Engine_ActorSetAnimation(3, 2);
    /* If the id-3 record lookup succeeds, forward its stored coordinates. */
    record = Engine_ActorGet(0);
    if (record != 0) {
        Engine_ActorSetDestination(3, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    Engine_ActorWaitForMove(3);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Audio_PlayCueFromEventWork();
    Event_End();
}
