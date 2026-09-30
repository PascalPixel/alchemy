#include "IRIGUCHI.H"
#include "CALL.H"
extern u8 MsgBabiSeemsLocked[];
extern u8 MsgBabiTheDoor[];
extern u8 MsgBabiYoureSureTheyWentThrough[];

void FieldScene_RunFourCallSequence(void)
{
    Event_Begin();
    StagedActor_AdvancePair();
    BabiIriguchi_JumpFromLedge();
    Event_End();
}

void SceneState_BranchOnActorEightOrNineTile(void)
{
    s32 *p = Actor_Get(9);

    if ((((s32 *)Engine_ActorGet(0))[2] >> 20) <= 12) {
        p = Actor_Get(8);
        if ((p[2] >> 20) == 6) {
            if ((p[4] >> 20) == 20) {
                FieldScene_RunFourCallSequence();
                return;
            }
        }
    } else {
        if ((p[2] >> 20) == 18) {
            if ((p[4] >> 20) == 20) {
                FieldScene_RunFourCallSequence();
                return;
            }
        }
    }
    FieldEffect_UpdateGridPlacement();
}

/* resource_3c5 owner at 0x02001158, 42 bytes. */
void ResetSceneParametersAndFinishSetup(void)
{
    ResetSceneParameters(-1, -1, -1, 0);
    BattleFx_RunRisingObjectSequence(0, 6, 0);
    Event_CloseScreen();
    Event_WaitForScreen();
}

void FieldScene_RunStep11(void)
{
    Event_Begin();
    ResetSceneParametersAndFinishSetup();
    Event_RequestExit(11);
    Event_End();
}

void FieldScene_RunStep12WithPosition(void)
{
    Event_Begin();
    OverlayObject_SpawnConfiguredObject(0x1d00000, 0, 0x1220000, 223);
    ResetSceneParametersAndFinishSetup();
    Event_RequestExit(12);
    Event_End();
}

void FieldScene_RunStep13WithTwoPositions(void)
{
    Event_Begin();
    OverlayObject_SpawnConfiguredObject(0x8f0000, 0, 0x1220000, 223);
    OverlayObject_SpawnConfiguredObject(0x790000, 0, 0x11e0000, 253);
    ResetSceneParametersAndFinishSetup();
    Event_RequestExit(13);
    Event_End();
}

void FieldScene_RunStep15(void)
{
    Event_Begin();
    ResetSceneParametersAndFinishSetup();
    Event_RequestExit(15);
    Event_End();
}

void FieldScene_RunStepWithValue2693(void)
{
    Event_Begin();
    Iriguchi_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered((s32)MsgBabiSeemsLocked, 1);
    Event_End();
}

void FieldScene_RunBranchingActorSequence(void)
{
    s32 record;

    GameFlag_Set(0x988);
    GameFlag_Set(0x98a);
    Event_Begin();
    Battle_ResetEffectCounter();
    Event_SetMessage((s32)MsgBabiYoureSureTheyWentThrough);
    Iriguchi_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x128, 0x160);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Iriguchi_Wait(10);
    Call4(Motion_LaunchFromFocusedObject, 10, 16, 0, 0xc000);
    Call4(Motion_LaunchFromFocusedObject, 1, -8, 16, 0xc000);
    Call4(Motion_LaunchFromFocusedObject, 2, 8, 16, 0xc000);
    Call4(Motion_LaunchFromFocusedObject, 3, 24, 16, 0xc000);
    Iriguchi_WaitForMove(ACTOR_MIA);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x1180000, -1, 0x1200000, 1);
    Camera_WaitForMove();
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(11, 3);
    Iriguchi_Wait(30);
    Event_ShowMessage(11, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0xc000, 0);
    Actor_FaceDirection(11, 0xc000, 0);
    Iriguchi_Wait(30);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_StartRepeatedMotion(13, 2);
    Actor_RunRepeatedMotion(12, 2);
    Iriguchi_Wait(40);
    Actor_StartRepeatedMotion(13, 2);
    Actor_RunRepeatedMotion(12, 2);
    Iriguchi_Wait(40);
    Actor_StartRepeatedMotion(13, 2);
    Actor_RunRepeatedMotion(12, 2);
    Iriguchi_Wait(40);
    Actor_ShowEmote(12, 0x102, 50);
    Event_ShowMessage(12, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(13, 0, 0);
    Iriguchi_Wait(25);
    Actor_RunRepeatedMotion(13, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(12, 0x8000, 0);
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(13, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(11, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(11, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(13, 0x107, 40);
    Iriguchi_Wait(10);
    Actor_FaceDirection(13, 0x4000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(13, 0x101, 75);
    Actor_ShowEmote(14, 0x101, 60);
    Actor_FaceDirection(12, 0x4000, 0);
    Iriguchi_Wait(20);
    Actor_FaceDirection(11, 0x4000, 0);
    Iriguchi_Wait(20);
    Actor_FaceDirection(14, 0x4000, 0);
    Iriguchi_Wait(30);
    Event_ShowMessage(14, 0);
    Camera_MoveTo(0x1180000, -1, 0x1400000, 1);
    Camera_WaitForMove();
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(10, 3);
    Iriguchi_Wait(30);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(10, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(14, 0x105, 60);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Iriguchi_Wait(20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Iriguchi_Wait(30);
        Actor_FaceDirection(10, 0x8000, 0);
        Iriguchi_Wait(30);
        Actor_SetAnimationAndWait(10, 3);
        Iriguchi_Wait(30);
        Event_ShowMessage(10, 0);
        bump_step_02001238(1);
    } else {
        Iriguchi_Wait(30);
        Actor_FaceDirection(10, 0x8000, 0);
        Iriguchi_Wait(30);
        Actor_SetAnimationAndWait(10, 4);
        Iriguchi_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(10, 0);
    }
    Iriguchi_Wait(10);
    Actor_ShowEmote(14, 0x101, 60);
    Iriguchi_SetSpeed(14, 0x10000, 0x8000);
    Actor_WalkByAndWait(14, 0, 16);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(10, 0xc000, 0);
    Iriguchi_Wait(35);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Iriguchi_Wait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(14, 0x100, 40);
    Event_OpenMessage(14, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Iriguchi_Wait(30);
        Actor_SetAnimationAndWait(14, 4);
        ((void (*)())Battle_WaitMode0)(20);
        Event_ShowMessage(14, 0);
        bump_step_02001238(1);
    } else {
        Iriguchi_Wait(30);
        ((void (*)())Engine_ActorSetAnimationAndWait)(14, 4);
        Iriguchi_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(14, 0);
    }
    Iriguchi_Wait(10);
    Actor_ShowEmote(10, 0x102, 50);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(13, 2);
    Iriguchi_Wait(20);
    Iriguchi_SetSpeed(13, 0x14ccc, 0xa666);
    Actor_WalkByAndWait(13, 0, 16);
    Iriguchi_Wait(20);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(10);
    Iriguchi_SetSpeed(12, 0x14ccc, 0xa666);
    Actor_WalkByAndWait(12, 0, 16);
    Iriguchi_Wait(20);
    Actor_ShowEmote(12, 0x107, 50);
    Event_ShowMessage(12, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Iriguchi_Wait(30);
    Iriguchi_Wait(10);
    Actor_ShowEmote(10, 0x102, 60);
    Actor_FaceDirection(10, 0x8000, 0);
    Iriguchi_Wait(25);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Iriguchi_Wait(20);
    Event_OpenMessage(10, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Iriguchi_Wait(30);
        Actor_SetAnimationAndWait(10, 3);
        ((void (*)())Battle_WaitMode0)(30);
        Event_ShowMessage(10, 0);
        bump_step_02001238(1);
    } else {
        Iriguchi_Wait(30);
        Actor_SetAnimationAndWait(10, 4);
        Iriguchi_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(10, 0);
    }
    Iriguchi_Wait(10);
    Actor_FaceDirection(10, 0xc000, 0);
    Iriguchi_Wait(35);
    Actor_SetAnimationAndWait(14, 3);
    Iriguchi_Wait(30);
    Actor_FaceDirection(14, 0xb000, 0);
    Iriguchi_Wait(40);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_FaceEachOther(12, 13, 50);
    Iriguchi_SetAnimation(12, 3);
    Actor_SetAnimationAndWait(13, 3);
    Iriguchi_Wait(30);
    Actor_FaceDirection(12, 0x4000, 0);
    Actor_FaceDirection(13, 0x4000, 0);
    Iriguchi_Wait(20);
    Iriguchi_SetAnimation(12, 3);
    Actor_SetAnimationAndWait(13, 3);
    Iriguchi_Wait(30);
    Iriguchi_SetSpeed(12, 0x10000, 0x8000);
    Iriguchi_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkBy(12, 32, 0);
    Actor_WalkByAndWait(13, 32, 0);
    Actor_WalkBy(12, 0, 16);
    Actor_WalkByAndWait(13, 16, 0);
    Actor_WalkTo(13, 0x158, 0x138);
    Actor_WalkToAndWait(12, 0x158, 0x150);
    Iriguchi_SetAnimation(13, 1);
    Actor_FaceDirection(12, 0x8000, 0);
    Actor_FaceDirection(13, 0x8000, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0x4000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Iriguchi_SetSpeed(14, 0x10000, 0x8000);
    Actor_WalkToAndWait(14, 0x148, 0x138);
    Actor_FaceDirection(14, 0x8000, 0);
    Iriguchi_Wait(30);
    Iriguchi_SetSpeed(11, 0x10000, 0x8000);
    Actor_WalkToAndWait(11, 0x148, 0x148);
    Actor_FaceDirection(11, 0x8000, 0);
    Iriguchi_Wait(20);
    Iriguchi_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Iriguchi_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
    Iriguchi_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Iriguchi_SetAnimation(ACTOR_GERALD, 2);
    record = Engine_ActorGet(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Iriguchi_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Iriguchi_SetAnimation(ACTOR_IVAN, 2);
    record = Engine_ActorGet(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Iriguchi_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Iriguchi_SetAnimation(ACTOR_MIA, 2);
    record = Engine_ActorGet(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Iriguchi_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Iriguchi_Wait(10);
    Event_End();
}

void FieldScene_RunActorEventSequence(void)
{
    s32 record;

    GameFlag_Set(0x989);
    Event_Begin();
    Battle_ResetEffectCounter();
    Event_SetMessage((s32)MsgBabiTheDoor);
    Iriguchi_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x128, 0x138);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Iriguchi_Wait(10);
    Motion_LaunchFromFocusedObject(1, 0, 16, 0);
    Call4(Motion_LaunchFromFocusedObject, 2, -16, -8, 0);
    Motion_LaunchFromFocusedObject(3, -16, 24, 0);
    Iriguchi_WaitForMove(ACTOR_MIA);
    Iriguchi_Wait(20);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x1180000, -1, 0x1480000, 1);
    Camera_WaitForMove();
    Iriguchi_Wait(10);
    Iriguchi_Wait(10);
    Actor_FaceDirection(10, 0xb000, 0);
    Iriguchi_Wait(10);
    Actor_ShowEmote(10, 0x100, 40);
    Event_ShowMessage(10, 0);
    Actor_Jump(10, 4, 13);
    Actor_Jump(10, 4, 30);
    Iriguchi_Wait(10);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_ShowEmote(12, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 40);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Iriguchi_Wait(30);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(13, 0x102, 40);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(12, 0x101, 50);
    Actor_FaceDirection(12, 0x8000, 0);
    Iriguchi_Wait(25);
    Event_ShowMessage(12, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0x8000, 0);
    Actor_FaceDirection(11, 0x8000, 0);
    Actor_FaceDirection(13, 0x8000, 0);
    Iriguchi_Wait(30);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Iriguchi_Wait(10);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(10, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(25);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Iriguchi_Wait(20);
    Iriguchi_SetSpeed(10, 0x10000, 0x8000);
    Actor_WalkByAndWait(10, 0, -40);
    Actor_FaceDirection(10, 0, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Iriguchi_Wait(30);
    Iriguchi_Wait(10);
    Actor_FaceDirection(13, 0x4000, 0);
    Actor_FaceDirection(12, 0xc000, 0);
    Iriguchi_Wait(30);
    Iriguchi_SetAnimation(12, 3);
    Actor_SetAnimationAndWait(13, 3);
    Iriguchi_Wait(30);
    Actor_FaceDirection(13, 0x8000, 0);
    Actor_FaceDirection(12, 0x8000, 0);
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(10, 3);
    Iriguchi_Wait(30);
    Camera_FollowActor(10, 1);
    Actor_WalkByAndWait(10, 0, -32);
    BabiIriguchi_CloseTruthDoor();
    GameFlag_Clear(0x301);
    Camera_MoveTo(-1, -1, -1, 0);
    Event_Begin();
    Actor_ShowEmote(10, 0x102, 40);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Iriguchi_Wait(30);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_ShowEmote(12, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 70);
    Camera_MoveTo(0x1180000, -1, 0x1380000, 1);
    Camera_WaitForMove();
    Iriguchi_Wait(10);
    Actor_ShowEmote(12, 0x102, 40);
    Event_ShowMessage(12, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(13, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Iriguchi_Wait(30);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(10, 0x4000, 0);
    Iriguchi_Wait(30);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    Iriguchi_Wait(30);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(14, 0x103, 50);
    Actor_FaceDirection(14, 0x8000, 0);
    Iriguchi_Wait(20);
    Event_OpenMessage(14, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(11, 0x8000, 0);
    Actor_FaceDirection(12, 0x8000, 0);
    Actor_FaceDirection(13, 0x8000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x1000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Iriguchi_Wait(30);
        Actor_RunRepeatedMotion(14, 2);
        Iriguchi_Wait(20);
        Event_ShowMessage(14, 0);
        bump_step_02001238(1);
    } else {
        Iriguchi_Wait(30);
        Actor_RunRepeatedMotion(14, 2);
        Iriguchi_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(14, 0);
    }
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Iriguchi_Wait(20);
    Actor_WalkByAndWait(10, 0, 16);
    Actor_FaceDirection(10, 0x2000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0xa000, 0);
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(14, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(10, 3);
    Iriguchi_Wait(30);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(20);
    Event_OpenMessage(14, 0);
    Iriguchi_Wait(40);
    Actor_FaceDirection(10, 0x5000, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(10, 0x101, 60);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Iriguchi_Wait(30);
        Actor_FaceDirection(10, 0x2000, 0);
        Iriguchi_Wait(20);
        Actor_SetAnimationAndWait(10, 3);
        Iriguchi_Wait(30);
        Event_ShowMessage(10, 0);
        bump_step_02001238(1);
    } else {
        Iriguchi_Wait(30);
        Actor_FaceDirection(10, 0x2000, 0);
        Iriguchi_Wait(20);
        Actor_SetAnimationAndWait(10, 4);
        Iriguchi_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(10, 0);
    }
    Iriguchi_Wait(10);
    Actor_ShowEmote(14, 0x102, 50);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(15);
    Actor_SetAnimationAndWait(10, 3);
    Iriguchi_Wait(30);
    Iriguchi_Wait(20);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(40);
    Actor_FaceDirection(14, 0x8000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Iriguchi_Wait(30);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(10, 0x5000, 0);
    Iriguchi_Wait(25);
    Actor_RunRepeatedMotion(10, 2);
    Iriguchi_Wait(20);
    Actor_WalkToAndWait(10, 0x138, 0x138);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(10, 0x8000, 0);
    Iriguchi_Wait(25);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Iriguchi_Wait(30);
    Iriguchi_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Iriguchi_SetAnimation(ACTOR_GERALD, 3);
    Iriguchi_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Iriguchi_Wait(30);
    Iriguchi_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Iriguchi_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
    Iriguchi_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Iriguchi_SetAnimation(ACTOR_GERALD, 2);
    record = Engine_ActorGet(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Iriguchi_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Iriguchi_SetAnimation(ACTOR_IVAN, 2);
    record = Engine_ActorGet(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Iriguchi_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Iriguchi_SetAnimation(ACTOR_MIA, 2);
    record = Engine_ActorGet(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Iriguchi_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(10, 0xb000, 0);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Iriguchi_Wait(30);
    Event_End();
}

void ActorPresentation_SetSceneCellByFlag985(void)
{
    if (GameFlag_IsSet(0x985) == 0) {
        s32 k5 = 17, k6 = 78;

        Engine_MapCopyCellsTo(36, 78, 1, 2, k5, k6);
    } else {
        s32 k5 = 17, k6 = 78;

        Engine_MapCopyCellsTo(34, 78, 1, 2, k5, k6);
    }
}

void SceneState_ApplyRectAt32x78(void)
{
    {
        s32 k5 = 17, k6 = 78;

        Engine_MapCopyCellsTo(32, 78, 1, 2, k5, k6);
    }
}

