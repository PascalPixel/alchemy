#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TEMPLE.H"

void Scene_RunActorNineTransition(void)
{
    Event_Begin();
    GameFlag_Set(2196);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessage(MSG_YOUNG_MASTER_DID_COMPLETE_TEST);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Scene_Call3(Engine_ActorFaceDirection, 0, 32768, 20);
    Event_AskYesNo(9, 0);
    Event_Wait(10);
    Scene_Call3(Engine_ActorShowEmote, 9, 256, 80);
    Scene_Call3(Engine_ActorFaceDirection, 9, 53248, 20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(9, 0, 20);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Scene_Call6(Engine_MapCopyCellAttributes, 10, 26, 1, 1, 10, 24);
    Event_End();
}

void Scene_RunScene39eSequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    rec7 = GameFlag_IsSet(0x300);
    if (rec7 != 0) {
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 0x1f8);
        Event_Wait(5);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
        *(u8 *)(((s32)Engine_ActorGet(8)) + 91) = 0;
        Audio_PlayCue(152);
        record = Actor_Get(8);
        *(s32 *)(record + 40) = 0x80000;
        Actor_SetAnimation(8, 1);
        Event_Wait(30);
        Event_SetMessage(MSG_MUST_SURVIVE_TEST_GROTTO_BEFORE);
    } else {
        Event_SetMessage(MSG_MMMM_WHO_WHO_SPEAKS_MY);
        FieldScene_SetFlag140AndFinishSequence(0, 8);
        Call1((void (*)())Engine_EventWait, 30);
        Event_ShowMessage(8, 0);
        FieldScene_FinishSequence();
        Event_Wait(20);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 0x1f8);
        Event_Wait(5);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
        Audio_PlayCue(152);
        *(u8 *)(((s32)Engine_ActorGet(8)) + 91) = rec7;
        record = Actor_Get(8);
        *(s32 *)(record + 40) = 0x80000;
        Actor_SetAnimation(8, 1);
        Event_Wait(30);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            Actor_RunRepeatedMotion(8, 2);
            Event_Wait(20);
            Event_ShowMessage(8, 0);
            ((void (*)())Engine_EventWait)(20);
            FieldScene_SetFlag140AndFinishSequence(8, 0);
            Event_Wait(30);
            Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Event_Wait(50);
            FieldScene_FinishSequence();
            Event_Wait(30);
            Actor_SetAnimationAndWait(8, 3);
            Event_ShowMessage(8, 0);
        } else {
            *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 2;
            Event_ShowMessage(8, 0);
        }
        Actor_SetAnimationAndWait(8, 3);
        Event_Wait(30);
        Actor_ShowEmote(8, 0x100, 60);
        Event_SetMessage(MSG_FOLLOW_THEM_DO_NOT);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            Actor_ShowEmote(8, 0x105, 60);
            FieldScene_SetFlag140AndFinishSequence(8, 0);
            Event_Wait(30);
            Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Event_Wait(50);
            FieldScene_FinishSequence();
            Event_Wait(30);
            Event_ShowMessage(8, 0);
            *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
        } else {
            *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
            Event_Wait(20);
            Actor_SetAnimationAndWait(8, 3);
            Event_Wait(20);
            Event_ShowMessage(8, 0);
        }
        Event_Wait(20);
        Actor_SetAnimationAndWait(8, 4);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
        Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
        Event_Wait(20);
        Actor_FaceDirection(8, 0xc000, 20);
        Actor_SetSpeed(8, 0x4ccc, 0x2666);
        Actor_WalkToAndWait(8, 168, 0x1d0);
        Event_Wait(60);
        Actor_FaceDirection(8, 0x4000, 40);
        Event_ShowMessageAndWait(8, 0, 10);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
        Actor_WalkToAndWait(8, 168, 0x1d8);
    }
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        Event_SetMessage(MSG_THEN_CANNOT_TELL_GIVE_UP);
        Event_ShowMessage(8, 0);
        GameFlag_Set(0x300);
    } else {
        Event_SetMessage(MSG_DO_NOT_WORRY_WILL_PERMITTED);
        Event_Wait(30);
        Actor_SetAnimationAndWait(8, 3);
        Event_Wait(20);
        Camera_SetSpeed(0x8000, 0x1000);
        Camera_MoveToActor(8, 1);
        ColorBuffer_ApplySource(0x10000, 0);
        ColorBuffer_ApplyTarget(0x10003, 1);
        ColorBuffer_Interpolate(30);
        Event_WaitForScreen();
        Camera_WaitForMove();
        FieldScene_SpawnEightShots();
        ColorBuffer_ApplyTarget(0x10000, 0);
        ColorBuffer_Interpolate(30);
        Event_ShowMessage(8, 0);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Event_Wait(60);
        Event_ShowMessageAndWait(8, 0, 10);
        GameFlag_Set(0x891);
    }
    Actor_SetAnimation(8, 5);
    Event_End();
}

void FieldScene_ShowDialogue17B1(void)
{
    Event_Begin();
    Event_SetMessage(MSG_ENJOY_READING_MINDS_OTHERS_DO);
    Event_AskYesNo(8, 0);
    Event_End();
}

void FieldScene_ShowDialogue1825(void)
{
    Event_Begin();
    Event_SetMessage(MSG_MASTER_FEHS_SCHOOL_CAME_WATCH);
    Event_AskYesNo(9, 0);
    Event_End();
}

void FieldScene_RunRoofSceneExit(void)
{
    Event_Begin();

    ((u8 *)Engine_ActorGet(12))[91] = 0;

    goto testPendingWork;
waitPendingWork:
        Task_Wait(1);
testPendingWork:
    if (*(s32 *)(((u8 *)Engine_ActorGet(12)) + 12) > 0) {
        goto waitPendingWork;
    }

    *(s32 *)(((u8 *)Engine_ActorGet(12)) + 12) = 0;

    *(s32 *)(((u8 *)Engine_ActorGet(12)) + 60) = 128 << 24;

    *(s32 *)(((u8 *)Engine_ActorGet(12)) + 40) = 0;

    ((u8 *)Engine_ActorGet(12))[91] = 1;

    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);

    if (GameFlag_IsSet(0x895) != 0) {
        Event_SetMessage(MSG_HSU_DID_NOT_PRACTICE_JUMPING);
    } else if (GameFlag_IsSet(0x89b) != 0) {
        Event_SetMessage(MSG_LAMA_TEMPLE_FAR_WEST_IN);
    } else {
        Event_SetMessage(MSG_FLEXIBILITY_JUMPING_VERY_IMPORTANT_IN);
    }

    Event_ShowMessage(12, 0);

    ((struct SceneRecordHeading *)Actor_Get(12))->heading = 128 << 7;

    ((u8 *)Engine_ActorGet(12))[91] = 0;

    Actor_EnableActionCallback(12, (u8 *)0x0200c638);
    Event_End();
}

void FieldScene_ShowDialogue182D(void)
{
    Event_Begin();
    Event_SetMessage(MSG_MASTER_FEH_VERY_BUSY_DO);
    Event_AskYesNo(15, 0);
    Event_End();
}

void FieldScene_RunForwardArcBurst(void)
{
    u8 *record = Actor_Get(19);
    u32 index;
    s32 angle;

    for (index = 8; index > 3; index--) {
        angle = index << 12;
        *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)angle;
        Task_Wait((index - 4) * 2);
        *(s32 *)(record + 8) += Math_Cos(angle)* 6;
        *(s32 *)(record + 16) += Math_Sin(angle)* 6;
    }

    *(s32 *)(record + 12) = 0x120000;
    *(s32 *)(record + 60) = 0x120000;

    Audio_PlayCue(227);

    Effect_Spawn(*(s32 *)(record + 8) - 0xc0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffffcccd, 0x6666, 0, 0, 0);
    Effect_Spawn(*(s32 *)(record + 8),
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff3334, 0x4ccc, 0, 0, 0);
    Effect_Spawn(*(s32 *)(record + 8) + 0xa0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff0000, 0x3333, 0, 0, 0);
}
