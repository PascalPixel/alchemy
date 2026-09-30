#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TEMPLE.H"
#include "CALL.H"

extern const struct SceneEntrance gShianJiinEntrances1[];
extern const struct SceneEntrance gShianJiinEntrancesOther[];

void FieldScene_PrepareActors(struct ScenePlacement *placements);
extern const struct ScenePlacement gShianJiinPlacements1[];
extern const struct ScenePlacement gShianJiinPlacementsEntrance3[];
extern struct ScenePlacement gShianJiinPlacements[];

extern u8 MsgShianHowDidGetHereBridge[];
extern u8 MsgShianIsntNobleHimTrySave[];
extern u8 MsgShianNowHeTrulyBeyondWorlds[];

extern u8 MsgShianAlreadyHeardTest[];
void Engine_EventBegin();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
void Engine_AudioPlayCue();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();
void Effect_Spawn();
void Engine_EventWait();
void BattleFx_PlayQueuedSoundFar();
void Engine_MapCopyCellAttributes();
void Engine_EventEnd();

extern u8 MsgShianDoNotWorryWillPermitted[];
extern u8 MsgShianEnjoyReadingMindsOthersDo[];
extern u8 MsgShianFlexibilityJumpingVeryImportantIn[];
extern u8 MsgShianFollowThemDoNot[];
extern u8 MsgShianHsuDidNotPracticeJumping[];
extern u8 MsgShianLamaTempleFarWestIn[];
extern u8 MsgShianMasterFehVeryBusyDo[];
extern u8 MsgShianMasterFehsSchoolCameWatch[];
extern u8 MsgShianMmmmWhoWhoSpeaksMy[];
extern u8 MsgShianMustSurviveTestGrottoBefore[];
extern u8 MsgShianThenCannotTellGiveUp[];
extern u8 MsgShianYoungMasterDidCompleteTest[];

union SceneWord {
    s32 word;
    u16 half[2];
};

void Engine_TaskWait();

extern u8 MsgShianCannotPushHands[];
extern u8 MsgShianGreatWarriorTrain[];
extern u8 MsgShianWarriorsCannotUse[];
void ShianJiin_WalkByFacing(void);

extern u8 MsgShianLookTreeFell[];
void Engine_ActorFaceActor();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorShowEmote();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorFaceEachOther();
void ShianJiin_WalkByFacing();
void Engine_ActorSetPosition();

extern u8 MsgShianDidDoWarrior[];
extern u8 MsgShianImTravelingAroundWorldSpread[];
extern u8 MsgShianItIsLocked[];
extern u8 MsgShianRobinAmCountingOnBring[];
extern u8 MsgShianWarriorWillShowMeYour[];
extern u8 MsgShianWaterMonstersFloodedAltinDid[];
extern u8 MsgShianYaTreeFell[];

extern const struct SceneEvent gShianJiinEvents1[];
extern const struct SceneEvent gShianJiinEventsEntrance3[];
extern const struct SceneEvent gShianJiinEvents[];

extern u8 MsgShianCanReadMindsKnowCurious[];
extern u8 MsgShianExcellentRobin[];
extern u8 MsgShianMonstersWaitInHidingWould[];
extern u8 MsgShianDontTryHard[];

extern u8 MsgShianMasterHamaMeditatingPleaseExtremely[];

void ShianJiin_AskIfCurious(void);
void FieldScene_RunRoofEnsembleSequence(void);
void *NewEffectObject(s32 x, s32 y, s32 z, s32 kind);
void BattleFx_SetQueuedSoundAndPlay(s32 value);

extern u8 gEffectWork[];

s32 StopXianActor(void *actor)
{
    Actor_SetSpriteFlags(actor, 0);
    return 0;
}

s32 FaceXianActorToPlayer(void *actor)
{
    void *player = Actor_Get(ACTOR_PARTY_LEADER);
    FIELD(actor, u16, 6) = ArcTan2Far(FIELD(player, s32, 0x10) - FIELD(actor, s32, 0x10), FIELD(player, s32, 8) - FIELD(actor, s32, 8));
    return 0;
}

/* Where the party appears in the scene it enters. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ShianJiin1) {
        return gShianJiinEntrances1;
    }
    return gShianJiinEntrancesOther;
}

s32 GetXianInitialState(void)
{
    return 0;
}

s32 GetXianMessageData(void)
{
    return (s32)ShianJiin_XianMessages;
}

/* The actors placed in the temple: the first scene and the third entrance
   have their own tables. Once flag 0x895 is set, the gated actors of the
   other table take that flag and one of them moves; that table is prepared
   before it is returned. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_ShianJiin1) {
        return gShianJiinPlacements1;
    }
    if (gGameState.entrance == 3) {
        return gShianJiinPlacementsEntrance3;
    }
    if (Value1(Engine_GameFlagIsSet, 0x895)) {
        gShianJiinPlacements[5].condition = 0x895;
        gShianJiinPlacements[7].condition = 0x895;
        gShianJiinPlacements[8].x = 0x1200000;
        gShianJiinPlacements[8].z = 0xf80000;
        gShianJiinPlacements[11].condition = 0x895;
        gShianJiinPlacements[12].condition = 0x895;
    }
    FieldScene_PrepareActors(gShianJiinPlacements);
    return gShianJiinPlacements;
}

void FieldScene_RunScene39e_02000414(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgShianHowDidGetHereBridge);
    if (GameFlag_IsSet(0x890) != 0) {
        bump_step(4);
    }
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        GameFlag_Set(0x890);
    } else {
        bump_step(1);
    }
    Event_ShowMessage(8, 0);
    Event_End();
}

void FieldScene_RunFlag88FBranch(void)
{
    extern u8 *gWork;

    Event_Begin();
    if (GameFlag_IsSet(0x88F) != 0) {
        Event_SetMessage((s32)MsgShianNowHeTrulyBeyondWorlds);
        Event_AskYesNo(12, 0);
        Event_End();
    } else {
        Event_SetMessage((s32)MsgShianIsntNobleHimTrySave);
        Event_OpenMessage(12, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            u16 *q = (u16 *)(gWork + 0x1D8);
            q[0] = q[0] + 1;
            Event_OpenMessage(12, 0);
            if (Event_ChooseYesNo(0, 0) == 1) {
                u16 *r = (u16 *)(gWork + 0x1D8);
                r[0] = r[0] + 1;
            }
        }
        Event_ShowMessage(12, 0);
        Event_End();
    }
}

void ShianJiin_RunTempleWalkScene(void)
{
    s32 *p8;
    s32 rec8;
    s32 record;
    u8 slot16[40];

    rec8 = (s32)Object_GetById(9);
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShianAlreadyHeardTest);
    Engine_EventShowMessageAndWait(9, 0, 20);
    Call3(Engine_ActorWalkToAndWait, 0, 168, 0x188);
    Engine_ActorFaceDirection(0, 0xc000, 20);
    Engine_AudioPlayCue(132);
    record = (s32)Object_GetById(9);
    *(s32 *)(record + 40) = 0x140000;
    record = (s32)Object_GetById(9);
    *(s32 *)(record + 72) = 0x40000;
    Call3(Engine_ActorSetSpeed, 9, 0x30000, 0x18000);
    Call3(Engine_ActorSetDestination, 9, 152, 0x188);
    Engine_ActorWaitForMove(9);
    record = (s32)Object_GetById(9);
    *(s32 *)(record + 72) = 0x10000;
    Engine_ActorFaceDirection(9, 0, 0);
    Engine_AudioPlayCue(132);
    p8 = (s32 *)slot16;
    p8[1] = 7;
    Effect_Spawn(*(s32 *)(rec8 + 8), *(s32 *)(rec8 + 12), (*(s32 *)(rec8 + 16) + 0x40000), 0x8000, 0, 0, 0x10000, p8);
    Effect_Spawn(*(s32 *)(rec8 + 8), *(s32 *)(rec8 + 12), (*(s32 *)(rec8 + 16) + 0x40000), 0, 0, 0, 0x10000, p8);
    Effect_Spawn(*(s32 *)(rec8 + 8), *(s32 *)(rec8 + 12), (*(s32 *)(rec8 + 16) + 0x40000), -0x8000, 0, 0, 0x10000, p8);
    Engine_EventWait(30);
    BattleFx_PlayQueuedSoundFar();
    Call6(Engine_MapCopyCellAttributes, 10, 24, 1, 1, 10, 22);
    Engine_GameFlagSet(0x892);
    Engine_EventEnd();
}

void Scene_RunActorNineTransition(void)
{
    Event_Begin();
    GameFlag_Set(2196);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessage((s32)MsgShianYoungMasterDidCompleteTest);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Call3(Engine_ActorFaceDirection, 0, 32768, 20);
    Event_AskYesNo(9, 0);
    Event_Wait(10);
    Call3(Engine_ActorShowEmote, 9, 256, 80);
    Call3(Engine_ActorFaceDirection, 9, 53248, 20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(9, 0, 20);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Call6(Engine_MapCopyCellAttributes, 10, 26, 1, 1, 10, 24);
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
        *(u8 *)(((s32)Object_GetById(8)) + 91) = 0;
        Audio_PlayCue(152);
        record = Actor_Get(8);
        *(s32 *)(record + 40) = 0x80000;
        Actor_SetAnimation(8, 1);
        Event_Wait(30);
        Event_SetMessage((s32)MsgShianMustSurviveTestGrottoBefore);
    } else {
        Event_SetMessage((s32)MsgShianMmmmWhoWhoSpeaksMy);
        FieldScene_SetFlag140AndFinishSequence(0, 8);
        ((void (*)())Engine_EventWait)(30);
        Event_ShowMessage(8, 0);
        FieldScene_FinishSequence();
        Event_Wait(20);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 0x1f8);
        Event_Wait(5);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
        Audio_PlayCue(152);
        *(u8 *)(((s32)Object_GetById(8)) + 91) = rec7;
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
        Event_SetMessage((s32)MsgShianFollowThemDoNot);
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
        Event_SetMessage((s32)MsgShianThenCannotTellGiveUp);
        Event_ShowMessage(8, 0);
        GameFlag_Set(0x300);
    } else {
        Event_SetMessage((s32)MsgShianDoNotWorryWillPermitted);
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
    Event_SetMessage((s32)MsgShianEnjoyReadingMindsOthersDo);
    Event_AskYesNo(8, 0);
    Event_End();
}

void FieldScene_ShowDialogue1825(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgShianMasterFehsSchoolCameWatch);
    Event_AskYesNo(9, 0);
    Event_End();
}

void FieldScene_RunRoofSceneExit(void)
{
    Event_Begin();

    ((u8 *)Object_GetById(12))[91] = 0;

    goto testPendingWork;
waitPendingWork:
        Task_Wait(1);
testPendingWork:
    if (*(s32 *)(((u8 *)Object_GetById(12)) + 12) > 0) {
        goto waitPendingWork;
    }

    *(s32 *)(((u8 *)Object_GetById(12)) + 12) = 0;

    *(s32 *)(((u8 *)Object_GetById(12)) + 60) = 128 << 24;

    *(s32 *)(((u8 *)Object_GetById(12)) + 40) = 0;

    ((u8 *)Object_GetById(12))[91] = 1;

    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);

    if (GameFlag_IsSet(0x895) != 0) {
        Event_SetMessage((s32)MsgShianHsuDidNotPracticeJumping);
    } else if (GameFlag_IsSet(0x89b) != 0) {
        Event_SetMessage((s32)MsgShianLamaTempleFarWestIn);
    } else {
        Event_SetMessage((s32)MsgShianFlexibilityJumpingVeryImportantIn);
    }

    Event_ShowMessage(12, 0);

    ((struct SceneRecordHeading *)Actor_Get(12))->heading = 128 << 7;

    ((u8 *)Object_GetById(12))[91] = 0;

    Actor_EnableActionCallback(12, ShianJiin_ActorTwelveScript);
    Event_End();
}

void FieldScene_ShowDialogue182D(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgShianMasterFehVeryBusyDo);
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

/* Spin actor 19 out along a quickening spiral, lift it and throw three puffs. */
void ShianJiin_SpinAway(void)
{
    struct FieldActor *actor;
    u32 i;
    s32 angle;

    actor = Object_GetById(19);
    for (i = 8; i <= 12; i++) {
        angle = i << 12;
        actor->sprite->rotation = angle;
        Engine_TaskWait((12 - i) * 2);
        actor->x.fixed -= Engine_MathCos(angle) * 6;
        actor->z.fixed -= Engine_MathSin(angle) * 6;
    }
    actor->y.fixed = 0x120000;
    actor->target_y = 0x120000;
    actor->scale_x = 0xffff3334;
    Engine_AudioPlayCue(227);
    Effect_Spawn(actor->x.fixed - 0xc0000, actor->y.fixed, actor->z.fixed + 0x80000, 0x10000, 0x3333, 0, 0, 0);
    Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0x80000, 0xcccc, 0x4ccc, 0, 0, 0);
    Effect_Spawn(actor->x.fixed + 0xa0000, actor->y.fixed, actor->z.fixed + 0x80000, 0x3333, 0x6666, 0, 0, 0);
}

void FieldScene_RunDescentBurst(void)
{
    u8 *record = Actor_Get(19);
    u32 i = 0;
    s32 step = 8;
    s32 zero;
    do {
        Task_Wait(step);
        *(s32 *)(record + 16) += 0xffff0000;
        *(u32 *)(record + 64) = 0x80000000;
        i++;
        step -= 2;
    } while (i <= 3);
    zero = 0;
    *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)zero;
    Audio_PlayCue(227);
    Effect_Spawn(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0xffff3334,
                  0, 0xffffcccd, 0, 0);
    Effect_Spawn(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0x0000cccc,
                  0, 0xffffcccd, 0, 0);
    Effect_Spawn(*(s32 *)(record + 8) + 0xfffa0000, *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
    Effect_Spawn(*(s32 *)(record + 8) + (192 << 11), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
}

void Scene_RunPrimarySequence(void)
{
    void *scene;
    u32 actor;
    s32 base;
    s32 slot;

    scene = (void *)Object_GetById(19);
    actor = 0;
    slot = 8;
    do {
        Engine_TaskWait(slot);
        ((union SceneWord *)scene)[4].word += 0x10000;
        *(s32 *)(scene + 64) = (s32)0x80000000;
        actor++;
        slot -= 2;
    } while (actor <= 3);

    ((union SceneWord *)*(u8 **)(scene + 80))[7].half[1] = 0;
    ((union SceneWord *)scene)[4].word += 0x180000;
    *(s32 *)(scene + 64) = (s32)0x80000000;
    Engine_AudioPlayCue(227);

    Effect_Spawn(
        *(s32 *)(scene + 8),
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) + 0xc0000,
        (s32)0xffff3334,
        0,
        0x3333,
        0,
        0);
    base = 0x3333;
    Effect_Spawn(
        *(s32 *)(scene + 8),
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) + 0xc0000,
        0xcccc,
        0,
        base,
        0,
        0);
    Effect_Spawn(
        *(s32 *)(scene + 8) - 0x60000,
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) - 0x80000,
        base,
        0,
        0x10000,
        0,
        0);
    Effect_Spawn(
        *(s32 *)(scene + 8) + 0x60000,
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) - 0x80000,
        base,
        0,
        0x10000,
        0,
        0);
}

/* Walk actor 15 around the leader by the quadrant it faces, then turn it. */
void ShianJiin_WalkByFacing(void)
{
    u32 dir;
    u32 limit;

    dir = *(u16 *)((s32)Object_GetById(0) + 6);
    limit = 0x3fff;
    if ((u32)((dir + -0x2000) << 16) <= 0x3fff0000) {
        Engine_ActorWalkToAndWait(15, 216, 168);
        Engine_ActorWalkToAndWait(15, 224, 168);
        Call3(Engine_ActorFaceDirection, 15, 0x2000, 20);
    } else if ((u16)(dir - 0x6000) <= limit) {
        Engine_ActorWalkToAndWait(15, 232, 160);
        Call3(Engine_ActorFaceDirection, 15, 0x5000, 20);
    } else if ((u16)(dir + 0x6000) <= limit) {
        Engine_ActorWalkToAndWait(15, 216, 168);
        Engine_ActorWalkToAndWait(15, 224, 172);
        Call3(Engine_ActorFaceDirection, 15, 0xe000, 20);
    } else {
        Engine_ActorWalkToAndWait(15, 232, 160);
        Call3(Engine_ActorFaceDirection, 15, 0x2000, 20);
    }
}

/* The Xian master's lines to his pupils before the training starts. */

/* Actor 15 speaks the lines for the given mode, then actors 19 and 20 are
 * placed and actor 15 walks up to them. The first mode's line is the one
 * before its name, as the game counts down from it in instructions. */
void ShianJiin_RunMasterScene(s32 mode)
{
    s32 msg;

    Call3((void (*)())Engine_ActorSetSpeed, 15, 0xcccc, 0x6666);
    Engine_EventWait(60);
    msg = (s32)MsgShianCannotPushHands;
    Engine_EventSetMessage(msg);
    if (mode == 0) {
        Engine_EventSetMessage(msg - 1);
        Call3((void (*)())Engine_ActorShowEmote, 15, 0x101, 60);
        Engine_EventShowMessageAndWait(15, 0, 20);
        Engine_ActorRunRepeatedMotion(15, 2);
        Engine_EventSetMessage((s32)MsgShianWarriorsCannotUse);
        Engine_EventShowMessageAndWait(15, 0, 20);
        Engine_ActorSetAnimationAndWait(15, 4);
        Engine_EventWait(20);
        Engine_EventShowMessageAndWait(15, 0, 20);
        Engine_ActorSetAnimationAndWait(15, 3);
        Engine_EventWait(20);
    }
    if (mode == 2) {
        Engine_EventSetMessage((s32)MsgShianGreatWarriorTrain);
        Engine_ActorRunRepeatedMotion(15, 2);
        Engine_EventWait(20);
    }
    Engine_EventShowMessageAndWait(15, 0, 20);
    ShianJiin_WalkByFacing();
    Engine_ActorRunRepeatedMotion(15, 3);
    Call3((void (*)())Engine_ActorSetPosition, 19, 0xe80000, 0xa80000);
    ((void (*)())Engine_ActorSetPosition)(20, 0xe80000, 0xa80000);
    Object_GetById(19)->y.fixed = 0xc0000;
    Object_GetById(19)->target_y = ACTOR_NO_TARGET;
    Object_GetById(19)->scale_x = 0xcccc;
    Object_GetById(19)->sprite->rotation = 0x8000;
    Engine_AudioPlayCue(124);
    Engine_EventWait(40);
    ((void (*)())Engine_ActorWalkToAndWait)(15, 216, 152);
    Call3((void (*)())Engine_ActorFaceDirection, 15, 0x2000, 30);
}

/* Xian: everyone turns to actor 19 and talks it through (MsgShianLookTreeFell
 * onward), actor 15 steps forward, actor 19 is lifted into place with cue
 * 124, and flag 0x301 is set. */
void ShianJiin_RunGatheringScene(void)
{
    u32 i;
    s32 record;

    Engine_ActorFaceActor(13, 19, 0);
    Engine_ActorFaceActor(14, 19, 0);
    Engine_ActorFaceActor(15, 19, 0);
    Engine_ActorFaceActor(16, 19, 0);
    Engine_ActorFaceActor(18, 19, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(15, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgShianLookTreeFell);
    Engine_EventShowMessageAndWait(15, 0, 20);
    ((void (*)())Engine_EventShowMessageAndWait)(16, 0, 20);
    Call3(Engine_ActorShowEmote, 18, 0x105, 60);
    Call3(Engine_ActorShowEmote, 16, 0x101, 60);
    Engine_EventShowMessageAndWait(16, 0, 20);
    Engine_ActorSetAnimationAndWait(18, 4);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(18, 0, 20);
    Call3(Engine_ActorShowEmote, 16, 0x102, 60);
    Engine_EventShowMessageAndWait(16, 0, 20);
    Engine_ActorFaceEachOther(15, 18, 0);
    Engine_EventWait(20);
    ((void (*)())Engine_ActorSetAnimationAndWait)(18, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(15, 3);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 15, 0xcccc, 0x6666);
    ShianJiin_WalkByFacing();
    Engine_ActorRunRepeatedMotion(15, 3);
    Call3(Engine_ActorSetPosition, 19, 0xe80000, 0xa80000);
    Engine_ActorSetPosition(20, 0xe80000, 0xa80000);
    record = (s32)Object_GetById(19);
    *(s32 *)(record + 12) = 0xc0000;
    record = (s32)Object_GetById(19);
    *(s32 *)(record + 60) = -0x80000000;
    record = (s32)Object_GetById(19);
    *(s32 *)(record + 24) = 0xcccc;
    record = (s32)Object_GetById(19);
    {
        s32 target = *(s32 *)(record + 80);
        s32 shown = 0x8000;
    
        *(u16 *)(target + 30) = shown;
    }
    Engine_AudioPlayCue(124);
    Engine_EventWait(40);
    Engine_ActorWalkToAndWait(15, 216, 152);
    Call3(Engine_ActorFaceDirection, 15, 0x4000, 30);
    Engine_GameFlagSet(0x301);
}

void FieldScene_DispatchApproachByFacing(void)
{
    Event_Begin();

    if (*(u16 *)(((u8 *)Object_GetById(0)) + 6) > (128 << 7)
        && *(u16 *)(((u8 *)Object_GetById(0)) + 6) < (192 << 8)) {
        FieldScene_RunForwardArcBurst();
    } else {
        ShianJiin_SpinAway();
    }

    if (GameFlag_IsSet(0x898) != 0) {
        ShianJiin_RunGatheringScene();
    } else {
        ShianJiin_RunMasterScene(0);
    }

    Event_End();
}

void FieldScene_DispatchByFacing(void)
{
    struct SceneActor_02001334 *record = Actor_Get(ACTOR_PARTY_LEADER);
    u16 angle;

    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 8);
    Event_Wait(20);

    angle = *(u16 *)((u8 *)record + 6);

    if ((u16)(angle - 0x2000) <= 0x3fffu) {
        Scene_RunPrimarySequence();
    } else if ((u16)(angle - 0x6000) <= 0x3fffu) {
        FieldScene_RunForwardArcBurst();
    } else if ((u16)(angle + (192 << 7)) <= 0x3fffu) {
        FieldScene_RunDescentBurst();
    } else {
        ShianJiin_SpinAway();
    }

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    ShianJiin_RunMasterScene(1);
    Event_End();
}

void FieldScene_DispatchByFacingAndFlags(void)
{
    u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
    u16 facing;

    Event_Begin();

    facing = *(u16 *)(record + 6);
    if ((u16)(facing - 0x2000) <= 0x3fff) {
        Scene_RunPrimarySequence();
    } else if ((u16)(facing - 0x6000) <= 0x3fff) {
        FieldScene_RunForwardArcBurst();
    } else if ((u16)(facing + 0x6000) <= 0x3fff) {
        FieldScene_RunDescentBurst();
    } else {
        ShianJiin_SpinAway();
    }

    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveToActor(20, 1);
    Camera_WaitForMove();

    if (*(s16 *)(record + 18) <= 209) {
        if (GameFlag_IsSet(0x89a) == 0) goto scene0;
        if (GameFlag_IsSet(0x89b) != 0) goto scene0;
        goto scene1;
scene0:
        ShianJiin_RunMasterScene(0);
        goto firstSceneComplete;
scene1:
        ShianJiin_RunGatheringScene();
firstSceneComplete:
        Event_End();
        return;
    }

    if (GameFlag_IsSet(0x89b) != 0) {
        ShianJiin_RunMasterScene(2);
    } else if (GameFlag_IsSet(0x89a) == 0) {
        FieldScene_RunSecondEnsembleBeat();
    } else {
        FieldScene_RunEnsembleStoryBeat();
    }
    Event_End();
}

void FieldScene_RunSecondEnsembleBeat(void)
{
    s32 id;
    u8 *rec;

    GameFlag_Set(0x89a);
    Event_Wait(30);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(15, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(16, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Actor_ShowEmote(13, 128 << 1, 0);
    Actor_ShowEmote(15, 128 << 1, 0);
    Actor_ShowEmote(16, 128 << 1, 0);
    Event_Wait(60);
    Event_SetMessage((s32)MsgShianDidDoWarrior);
    Event_ShowMessageAndWait(13, 0, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Actor_RunRepeatedMotion(15, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 15, 0);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 16, 0);
    Event_AskYesNo(16, 0);
    Event_Wait(50);
    Camera_FollowActor(16, 1);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkToAndWait(16, 176, 248);
    Actor_WalkToAndWait(16, 154 << 1, 248);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 128 << 6, 0);
    Actor_FaceDirection(16, 192 << 8, 20);
    Audio_PlayCue(158);
    Map_AnimateCells((const u16 *)ShianJiin_CellStepsB, 78, 13);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Actor_SetSpeed(16, 192 << 9, 192 << 8);
    rec = Object_GetById(16);
    rec[90] &= 0xfe;
    Actor_WalkToAndWait(16, 154 << 1, 136 << 1);
    Event_Wait(1);
    rec = ((u8 *)Value1(Object_GetById, 16));
    {
        /*
         * A result temporary, not the compound or-assign the matching
         * &= 0xfe case above uses. The reference writes the result into
         * the mask register rather than the loaded value, and the
         * two-address ORR only does that when the merged result is its
         * own object; the compound form keeps the loaded value as
         * destination. Same technique already adopted in the sibling
         * owner resource_3bd:020013f8.
         */
        u8 merged = (u8)(rec[90] | 1);

        rec[90] = merged;
    }
    Event_ShowMessageAndWait(16, 0, 50);
    Actor_SetPosition(17, 152 << 17, 216 << 16);
    Actor_WalkToAndWait(17, 152 << 1, 248);
    Actor_FaceActor(9, 17, 0);
    Actor_FaceActor(10, 17, 0);
    Actor_FaceActor(11, 17, 0);
    Actor_FaceActor(12, 17, 0);
    Actor_FaceActor(13, 17, 0);
    Actor_FaceActor(14, 17, 0);
    Actor_FaceActor(15, 17, 0);
    Actor_FaceActor(16, 17, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 17, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(9, 2);
    Actor_StartRepeatedMotion(10, 2);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Actor_StartRepeatedMotion(13, 2);
    Actor_StartRepeatedMotion(14, 2);
    Actor_StartRepeatedMotion(15, 2);
    Actor_RunRepeatedMotion(16, 2);
    Actor_ShowEmote(17, 0x103, 60);
    Actor_SetPosition(18, 152 << 17, 216 << 16);
    Actor_WalkTo(18, 152 << 1, 248);
    Actor_WalkTo(17, 140 << 1, 132 << 1);
    Actor_WaitForMove(18);
    Actor_FaceDirection(18, 160 << 7, 0);
    Actor_WaitForMove(17);
    Audio_PlayCue(159);
    Map_AnimateCells((const u16 *)ShianJiin_CellStepsC, 78, 13);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceActor(9, 17, 0);
    Actor_FaceActor(10, 17, 0);
    Actor_FaceActor(11, 17, 0);
    Actor_FaceActor(12, 17, 0);
    Actor_FaceActor(13, 17, 0);
    Actor_FaceActor(14, 17, 0);
    Actor_FaceActor(15, 17, 0);
    Actor_FaceActor(16, 17, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 17, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(17, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(18, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceDirection(17, 208 << 8, 20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_ShowEmote(18, 0x102, 60);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_RunRepeatedMotion(17, 2);
    Event_Wait(20);
    Actor_FaceDirection(17, 0, 20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_FaceDirection(16, 128 << 8, 20);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_FaceDirection(17, 128 << 8, 20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(17, 208 << 8, 20);
    Actor_RunRepeatedMotion(17, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_ShowEmote(18, 0x102, 60);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(17, 0x101, 60);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(17, 0x100, 60);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Actor_ShowEmote(17, 0x103, 60);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_ShowEmote(18, 0x100, 60);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_RunRepeatedMotion(17, 2);
    Event_Wait(10);
    Actor_WalkToAndWait(17, 128 << 1, 140 << 1);
    Actor_FaceDirection(17, 128 << 7, 20);
    Actor_SetPosition(17, 0, 0);
    Actor_Destroy(17);
    Event_Wait(30);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_RunRepeatedMotion(15, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceActor(16, 18, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_FaceActor(18, 16, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    Actor_WalkToAndWait(18, 128 << 1, 248);
    Actor_FaceDirection(18, 192 << 8, 20);
    Actor_StartRepeatedMotion(18, 1);
    Actor_ShowEmote(18, 0x100, 60);
    Actor_WalkToAndWait(18, 240, 184);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Actor_SetPosition(19, 232 << 16, 168 << 16);
    Actor_SetPosition(20, 232 << 16, 168 << 16);
    rec = ((u8 *)Value1(Object_GetById, 19));
    *(s32 *)(rec + 12) = 0xc0000;
    rec = ((u8 *)Value1(Object_GetById, 19));
    *(s32 *)(rec + 60) = -0x80000000;
    rec = ((u8 *)Value1(Object_GetById, 19));
    *(s32 *)(rec + 24) = 0xcccc;
    rec = ((u8 *)Value1(Object_GetById, 19));
    {
        u8 *target = *(u8 **)(rec + 80);
        s32 shown = 0x8000;

        *(u16 *)(target + 30) = shown;
    }
    Audio_PlayCue(124);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 192 << 8, 20);
    Actor_WalkToAndWait(16, 128 << 1, 240);
    Actor_FaceDirection(16, 176 << 8, 20);
    Actor_RunRepeatedMotion(16, 1);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(15, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(16, ACTOR_PARTY_LEADER, 0);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Actor_FaceDirection(18, 160 << 7, 20);
    Actor_WalkToAndWait(18, 248, 208);
    Actor_FaceDirection(18, 160 << 7, 20);
    Event_OpenMessage(18, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        ((s32 (*)())Engine_ActorRunRepeatedMotion)(16, 1);
        Event_Wait(20);
        id = 16;
        goto joinBeat;
    }

    /* Skipped once: bump the workspace skip counter and offer the beat again. */
    Scene_BumpStep(1);
    Event_Wait(20);
    Actor_ShowEmote(18, 0x105, 60);
    Actor_FaceDirection(18, 128 << 7, 20);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Event_OpenMessage(16, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        goto skipTwice;
    }
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_FaceDirection(18, 176 << 8, 20);
    id = 18;

joinBeat:
    Event_ShowMessageAndWait(id, 0, 20);
    GameFlag_Set(0x898);
    goto finish;

skipTwice:
    Scene_BumpStep(1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    GameFlag_Set(0x899);

finish:
    Actor_FaceDirection(10, 128 << 8, 0);
    Actor_FaceDirection(11, 128 << 8, 20);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Engine_ActorEnableActionCallback(12, ShianJiin_ActorTwelveScript);
}

void FieldScene_RunSkippableStoryBeat(void)
{
    extern u8 *gWork;

    u8 *workspace;

    Event_Begin();
    Event_SetMessage((s32)MsgShianWarriorWillShowMeYour);
    Event_OpenMessage(18, 0);

    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_ShowMessageAndWait(18, 0, 20);
        GameFlag_Set(0x898);
        Event_End();
    } else {
        workspace = gWork;
        *(u16 *)(workspace + 472) += 1;
        Event_ShowMessageAndWait(18, 0, 20);
        Event_End();
    }
}

/* Looks up actors 18, 13, 14, 15 and 16 and clears their +108 field before
 * the scene runs. */
void FieldScene_RunEnsembleStoryBeat(void)
{
    u32 i;
    s32 actor;

    actor = Actor_Get(18);
    ACTOR_FIELD_108(actor) = 0;
    actor = Object_GetById(13);
    ACTOR_FIELD_108(actor) = 0;
    actor = Object_GetById(14);
    ACTOR_FIELD_108(actor) = 0;
    actor = Object_GetById(15);
    ACTOR_FIELD_108(actor) = 0;
    actor = Actor_Get(16);
    ACTOR_FIELD_108(actor) = 0;
    Engine_ActorSetAnimation(11, 1);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0xe80000, -1, 0xc80000, 1);
    Camera_WaitForMove();
    Event_SetMessage((s32)MsgShianYaTreeFell);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_SetSpeed(12, 0xcccc, 0x6666);
    Actor_WalkTo(10, 152, 200);
    Actor_WalkToAndWait(12, 144, 248);
    Actor_WaitForMove(10);
    Actor_FaceActor(9, 19, 0);
    Actor_FaceActor(11, 19, 0);
    Actor_FaceActor(13, 19, 0);
    Actor_FaceActor(14, 19, 0);
    Actor_FaceActor(15, 19, 0);
    Actor_FaceActor(16, 19, 0);
    Actor_FaceActor(18, 19, 0);
    Actor_SetSpeed(10, 0x18000, 0xc000);
    Actor_SetSpeed(12, 0x20000, 0x10000);
    Actor_WalkTo(10, 152, 200);
    Actor_WalkToAndWait(12, 144, 248);
    Actor_FaceActor(12, 19, 0);
    Actor_WaitForMove(10);
    Actor_FaceActor(10, 19, 0);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 40);
    Actor_FaceActor(9, 18, 0);
    Actor_FaceActor(10, 18, 0);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceActor(12, 18, 0);
    Actor_FaceDirection(13, 0x3000, 0);
    Actor_FaceActor(14, 18, 0);
    Actor_FaceActor(15, 18, 0);
    Actor_FaceActor(16, 18, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_ShowEmote(18, 0x105, 60);
    Actor_ShowEmote(16, 0x101, 60);
    Event_ShowMessageAndWait(16, 0, 20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(16, 0x102, 60);
    Actor_ShowEmote(15, 0x101, 60);
    Actor_SetSpeed(15, 0xcccc, 0x6666);
    Actor_WalkToAndWait(15, 216, 176);
    Actor_FaceDirection(15, 0x3000, 20);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceDirection(18, 0xb000, 20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_StartRepeatedMotion(9, 2);
    Actor_StartRepeatedMotion(10, 2);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Actor_StartRepeatedMotion(13, 2);
    Actor_StartRepeatedMotion(14, 2);
    Actor_StartRepeatedMotion(15, 2);
    Actor_StartRepeatedMotion(16, 2);
    Event_Wait(40);
    Actor_RunRepeatedMotion(13, 2);
    Event_ShowMessageAndWait(13, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
    Actor_FaceDirection(18, 0x5000, 20);
    Event_AskYesNo(18, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(10, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(11, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(12, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(13, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(14, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(15, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(16, 0x101, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(16, 2);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(15, 0x101, 60);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceActor(18, 15, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 40);
    Actor_FaceEachOther(11, 10, 0);
    Actor_FaceEachOther(12, 14, 0);
    Actor_FaceEachOther(13, 15, 0);
    Event_Wait(60);
    Actor_FaceActor(10, 18, 0);
    Actor_FaceActor(11, 18, 0);
    Actor_FaceActor(12, 18, 0);
    Actor_FaceActor(13, 18, 0);
    Actor_FaceActor(14, 18, 0);
    Actor_FaceActor(15, 18, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(13, 3);
    Actor_SetAnimation(14, 3);
    Actor_SetAnimation(15, 3);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_FaceDirection(18, 0x5000, 20);
    Event_AskYesNo(18, 0);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(13, 3);
    Actor_SetAnimation(14, 3);
    Actor_SetAnimation(15, 3);
    ObjectMotion_CallThenWaitForAnimationChange_9(16, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Actor_FaceDirection(18, 0x8000, 20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(13, 3);
    Actor_SetAnimation(14, 3);
    Actor_SetAnimation(15, 3);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_WalkTo(10, 120, 200);
    Actor_WalkTo(12, 120, 248);
    Actor_WaitForMove(10);
    Actor_FaceDirection(11, 0x8000, 20);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Actor_WaitForMove(12);
    Engine_ActorEnableActionCallback(12, (s32)ShianJiin_ActorTwelveScript);
    Actor_SetSpeed(15, 0xcccc, 0x6666);
    Actor_WalkToAndWait(15, 216, 168);
    Actor_WalkToAndWait(15, 232, 168);
    Actor_FaceDirection(15, 0xc000, 20);
    Actor_RunRepeatedMotion(15, 3);
    Actor_SetPosition(19, 0xe80000, 0xa80000);
    actor = Actor_Get(19);
    *(s32 *)(actor + 12) = 0xc0000;
    actor = Actor_Get(19);
    *(s32 *)(actor + 60) = -0x80000000;
    actor = Actor_Get(19);
    {
        s32 target = *(s32 *)(actor + 80);
        s32 shown = 0x8000;

        *(u16 *)(target + 30) = shown;
    }
    Audio_PlayCue(124);
    Event_Wait(40);
    Actor_WalkToAndWait(15, 216, 152);
    Actor_FaceDirection(15, 0x4000, 30);
    GameFlag_Clear(0x898);
    GameFlag_Set(0x89b);
}

void FieldScene_ShowDialogue1A58(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgShianWaterMonstersFloodedAltinDid);
    Event_AskYesNo(11, 0);
    Event_End();
}

void StartSchoolDoorEvent(void)
{
    Event_Begin();
    if (GameFlag_IsSet(2202) == 0 && GameFlag_IsSet(2197) == 0) {
        Message_ShowCentered((s32)MsgShianItIsLocked, 1);
        Event_End();
    } else {
        Audio_PlayCue(158);
        Map_AnimateCells((const u16 *)ShianJiin_CellStepsB, 78, 13);
        SetScale(0, 0x8000, 0x4000);
        SetPosition(0, 306, 248);
        Actor_WalkTo(ACTOR_PARTY_LEADER, 304, 216);
        Event_Wait(20);
        Event_RequestExit(4);
        Event_End();
    }
}

void FieldScene_DispatchByRange(void)
{
    u8 *record;
    u32 biased;

    record = Actor_Get(ACTOR_PARTY_LEADER);
    biased = *(u16 *)(record + 6);
    Event_Begin();

    biased = biased + 0xffff5fff;
    if (biased <= 0x3ffe) {
        Sanctum_Open(13);
    } else {
        Event_SetMessage((s32)MsgShianImTravelingAroundWorldSpread);
        Event_ShowMessage(13, 0);
    }

    Event_End();
}

void FieldScene_ShowDialogue17DF(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(8, 2);
    Event_SetMessage((s32)MsgShianRobinAmCountingOnBring);
    Event_ShowMessage(8, 0);
    Event_End();
}

/* What the temple answers: the first scene and the third entrance have
   their own events. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_ShianJiin1) {
        return gShianJiinEvents1;
    }
    if (gGameState.entrance == 3) {
        return gShianJiinEventsEntrance3;
    }
    return gShianJiinEvents;
}

void FieldScene_SpawnRandomizedParticle(void)
{
    struct Params params;
    u8 *record;
    s32 draw;
    s32 offset;

    record = Actor_Get(ACTOR_PARTY_LEADER);

    params.field1 = 7;
    draw = (u32)(Random_Next() * 7) >> 16;
    if ((draw & 7) == 0)
        params.field1 = 5;

    params.field2 = 0xb333;
    params.field3 = 0xcccc;

    offset = ((u32)(Random_Next() * 8) >> 16) * 13107;

    Effect_Spawn(*(s32 *)(record + 8) + ((8 - (gFrameCount & 15)) << 16),
                  *(s32 *)(record + 12) + (192 << 13),
                  *(s32 *)(record + 16),
                  0,
                  -offset,
                  0,
                  144 << 12,
                  (u8 *)&params);

    if ((gFrameCount & 1) != 0)
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    else
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 1);
}

void FieldScene_ApplyOffset0Neg32(void)
{
    FieldScene_RunOpeningAuxiliarySequence(0, -32);
}

void FieldScene_ApplyOffset0Pos32(void)
{
    FieldScene_RunOpeningAuxiliarySequence(0, 32);
}

void FieldScene_ApplyOffsetNeg32_0(void)
{
    FieldScene_RunOpeningAuxiliarySequence(-32, 0);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
    Engine_ActorSetDestinationOffset(0, a0, a1);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 7);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 6);
    Event_End();
}

void FieldScene_RunScene39eSequenceA(void)
{
    u32 i;
    s32 record;
    s32 base5_200a5b9;

    Event_Begin();
    base5_200a5b9 = (s32)FieldScene_SpawnRandomizedParticle;
    Engine_ScheduleCallbackFar(base5_200a5b9, 0xc80);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
    gEventWork->transition_frames = 60;
    Event_CloseScreen();
    Audio_PlayCue(154);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(record, 0);
    Scheduler_RemoveCallbackFar(base5_200a5b9);
    Event_WaitForScreen();
    Event_RequestExit(3);
    Event_End();
}

void FieldScene_PlaySound123AndEnable(void)
{
    Audio_PlayCue(123);
    Event_RequestExit(1);
}

void FieldScene_RunScene39e_02002778(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Audio_PlayCue(188);
    Map_AnimateCells((const u16 *)ShianJiin_CellStepsA, 77, 8);
    *(u8 *)(((s32)Object_GetById(0)) + 85) = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    Event_Wait(16);
    Event_RequestExit(2);
    Event_End();
}

/* The party walks in and actor 8, who can read minds, asks whether it is
 * curious. */
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
    ((u8 *)Object_GetById(8))[91] = 0;
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

/* Long scripted sequence: sets up and steps a series of actors (indices 0-3,
 * 8-10, 17, 20) through position, pose, animation, wait, and flag-bit calls,
 * copying a couple of fields between some actors' records along the way. */
void FieldScene_RunRoofEnsembleSequence(void)
{
    u32 unused;
    u8 *source_record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1d8, 0x218);
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(9, 0, 20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_SetMessage((s32)MsgShianMasterHamaMeditatingPleaseExtremely);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(30);
    Actor_FaceDirection(8, 0x4000, 30);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Event_Wait(20);
    Actor_FaceDirection(9, 0xd000, 0);
    /* Copy the pair of s32 fields at +8/+16 from actor 0's record (as seen
     * through each accessor) into the matching setter for another actor. */
    source_record = Actor_Get(ACTOR_PARTY_LEADER);
    if (source_record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    source_record = Actor_Get(ACTOR_PARTY_LEADER);
    if (source_record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    source_record = Actor_Get(ACTOR_PARTY_LEADER);
    if (source_record != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_IVAN, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_MIA, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1d0, 0x1f8);
    Actor_WalkTo(ACTOR_IVAN, 0x1e0, 0x1f8);
    Actor_WalkTo(ACTOR_GERALD, 0x1f0, 0x1f0);
    Actor_WalkTo(ACTOR_MIA, 0x1c0, 0x1f0);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 20);
    Event_AskYesNo(ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 20);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 60);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 0);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 30);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Actor_FaceActor(8, ACTOR_IVAN, 0);
    Event_Wait(10);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 60);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(8, 0x100, 60);
    Actor_FaceActor(8, ACTOR_GERALD, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 60);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_FaceActor(8, ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(8, 0x5000, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 0);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(60);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(8, 0x102, 0);
    Actor_StartRepeatedMotion(8, 1);
    Event_Wait(60);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 20);
    Event_AskYesNo(ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 20);
    Actor_FaceDirection(8, 0xc000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_FaceDirection(8, 0x4000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xd000, 0);
    Event_Wait(20);
    Event_AskYesNo(8, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Event_AskYesNo(ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 30);
    Actor_FaceDirection(8, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_FaceDirection(8, 0xc000, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    Event_Wait(30);
    Actor_FaceDirection(8, 0x4000, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetPosition(10, 0x1d80000, 0x2600000);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Actor_ShowEmote(8, 0x100, 0);
    Actor_ShowEmote(9, 0x100, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 0);
    Actor_FaceDirection(9, 0, 0);
    Event_Wait(30);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkToAndWait(10, 0x1d8, 0x218);
    Actor_ShowEmote(8, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 40);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x1e8, 0x200);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 30);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1c8, 0x200);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 20);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_ShowEmote(8, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(10, 2);
    Actor_SetAttachedEffect(10, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_ShowEmote(8, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(10, 0x100, 60);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_StartRepeatedMotion(8, 1);
    Actor_ShowEmote(8, 0x100, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(8, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(17, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(8, 0x100, 30);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Event_Wait(2);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Event_Wait(1);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Event_Wait(5);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkToAndWait(10, 0x1d8, 0x1f8);
    Event_Wait(20);
    Actor_StartRepeatedMotion(10, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_FaceActor(8, 10, 0);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 0);
    Actor_WaitForMove(8);
    Event_Wait(30);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_WalkTo(10, 0x1d8, 0x238);
    Actor_WalkToAndWait(8, 0x1d8, 0x218);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_WaitForMove(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_StartRepeatedMotion(8, 2);
    Actor_ShowEmote(8, 0x102, 60);
    Call3((void (*)())Engine_ActorFaceDirection, 10, 0xd000, 0);
    Actor_FaceDirection(8, 0xd000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Actor_WalkToAndWait(8, 0x1d8, 0x200);
    Actor_FaceDirection(8, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_FaceDirection(9, 0xd000, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    /* Clear bit 0 of the flag byte at +90. */
    ((u8 *)Object_GetById(0))[90] &= 0xfe;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1c0, 0x200);
    Event_Wait(1);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
        u8 value = record[90] | 1;

        record[90] = value;
    }
    Event_Wait(20);
    FieldScene_RunParticleRain();
    Event_Wait(60);
    UiText_DrawQuantityPairWithCueFar(2, 144);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 0);
    Actor_FaceDirection(9, 0x3000, 0);
    Event_Wait(20);
    Actor_WalkToAndWait(8, 0x1d8, 0x228);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Actor_FaceDirection(8, 0xd000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetSpeed(8, 0x8000, 0x4000);
    Actor_WalkToAndWait(8, 0x1e0, 0x21c);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_WalkTo(8, 0x1d8, 0x260);
    Actor_WalkTo(9, 0x1d8, 0x220);
    Actor_WalkToAndWait(10, 0x1d8, 0x260);
    Actor_WaitForMove(9);
    Actor_WalkTo(9, 0x1d8, 0x260);
    Actor_SetPosition(10, 0, 0);
    Actor_WaitForMove(8);
    Actor_SetPosition(8, 0, 0);
    Actor_WaitForMove(9);
    Actor_SetPosition(9, 0, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x1e8, 0x208);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Event_AskYesNo(ACTOR_GERALD, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Event_OpenMessage(ACTOR_MIA, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        /* Same step counter as bump_step(), incremented inline here. */
        *(u16 *)((*(u8 **)&gEventWork + 0x1d8)) += 1;
    } else {
        Event_Wait(20);
        /* Same step counter as bump_step(), incremented inline here. */
        *(u16 *)((*(u8 **)&gEventWork + 0x1d8)) += 1;
        Actor_SetAnimationAndWait(ACTOR_MIA, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    }
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_WalkTo(ACTOR_GERALD, 0x1c0, 0x200);
    Actor_WalkToAndWait(ACTOR_MIA, 0x1c0, 0x200);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 20);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x1c0, 0x200);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Event_End();
    GameFlag_Set(0x895);
}

/* Temple entry: in the second scene set the entrance selector and, by entrance and story flags, stage the roof and gathering actors; elsewhere restore the opened passages. */
s32 ShianJiin_ApplyEntryState(void)
{
    struct FieldActor *actor;
    s32 set;

    if (gGameState.scene == (s32)&SceneId_ShianJiin2) {
        gEventWork->start_transition = 0x209;
        if (gGameState.entrance == 1) {
            if (Engine_GameFlagIsSet(0x88f)) {
                Engine_ActorSetAnimation(8, 6);
            } else {
                Engine_ActorSetAnimation(8, 5);
                if (Engine_GameFlagIsSet(0xf14) && !Engine_GameFlagIsSet(0x893) && !Engine_GameFlagIsSet(0x109)) {
                    ShianJiin_AskIfCurious();
                }
            }
        } else if (gGameState.entrance == 2 || gGameState.entrance == 4) {
            Engine_GameFlagClear(0x12f);
            set = Engine_GameFlagIsSet(0x895);
            if (set == 0) {
                actor = Object_GetById(19);
                actor->motion_flags = set;
                actor->y.fixed = 0xc0000;
                actor->target_y = 0xc0000;
                actor->scale_x = 0xcccc;
                actor->scale_y = 0x8000;
                actor->sprite->rotation = 0x8000;
                if (Engine_GameFlagIsSet(0x89a)) {
                    Engine_ActorSetPosition(18, 0xf80000, 0xd00000);
                    if (!Engine_GameFlagIsSet(0x89b)) {
                        Engine_ActorSetPosition(16, 0x1000000, 0xf00000);
                        Object_GetById(18)->update = FaceXianActorToPlayer;
                        Object_GetById(13)->update = FaceXianActorToPlayer;
                        Object_GetById(14)->update = FaceXianActorToPlayer;
                        Object_GetById(15)->update = FaceXianActorToPlayer;
                        Object_GetById(16)->update = FaceXianActorToPlayer;
                    }
                }
            } else {
                actor = Object_GetById(19);
                actor->motion_flags = 0;
                actor->y.fixed = 0xc0000;
                actor->target_y = 0xc0000;
                actor->scale_x = 0xcccc;
                actor->scale_y = 0x8000;
                actor->collision_flags |= 8;
                actor->sprite->rotation = 0x8000;
                Call6(Engine_MapCopyCellAttributes, 14, 11, 1, 1, 14, 10);
            }
            NewEffectObject(0x1300000, 0x180000, 0xe00000, 223);
            Engine_ActorSetAnimation(10, 5);
            Engine_ActorSetAnimation(11, 5);
        } else if (gGameState.entrance == 3) {
            Engine_GameFlagClear(0x12f);
            if (!Engine_GameFlagIsSet(0x895)) {
                FieldScene_RunRoofEnsembleSequence();
            } else if (!Engine_GameFlagIsSet(0x8b2)) {
                Engine_ActorSetPosition(8, 0, 0);
                Engine_ActorSetPosition(9, 0, 0);
            }
        }
    } else {
        BattleFx_SetQueuedSoundAndPlay(170);
        Object_GetById(9)->collision_flags |= 16;
        if (gGameState.entrance == 3 && Engine_GameFlagIsSet(0xf14) && !Engine_GameFlagIsSet(0x894)) {
            Call6(Engine_MapCopyCellAttributes, 10, 84, 1, 1, 10, 24);
        }
        if (Engine_GameFlagIsSet(0x892)) {
            Call3(Engine_ActorSetPosition, 9, 0x980000, 0x1880000);
            Engine_ActorFaceDirection(9, 0, 0);
            Call6(Engine_MapCopyCellAttributes, 10, 26, 1, 1, 10, 22);
        }
    }
    return 0;
}

void FieldScene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1)
{
    u8 *globalCtx;

    GameFlag_Set(160 << 1);
    Psynergy_Begin(141, 1);
    globalCtx = *(u8 **)gEffectWork;
    Psynergy_SetTarget(arg0, arg1);
    globalCtx[0x23] = 0;
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Task_Wait(1);
}

void FieldScene_FinishSequence(void)
{
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Psynergy_PlayEffect(2);
    Psynergy_LowerHands();
}

void FieldScene_SpawnEightShots(void)
{
    struct Descriptor_02000484 descriptor;
    u8 *record;
    u32 i;

    record = Actor_Get(8);
    descriptor.field0 = 1;
    descriptor.field24 = 0x0119;
    descriptor.field28 = 0x0200d1d8;
    descriptor.field16 = 224 << 10;
    descriptor.field20 = 192 << 9;
    for (i = 0; i <= 7; i++) {
        Event_Wait(10);
        if (i & 1) {
            Audio_PlayCue(0x82);
        }
        Effect_Spawn(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16) + 0xffe80000, 0,
                      0x9999, 0, 0x00360001, (u8 *)&descriptor);
    }
    Event_Wait(60);
}

void FieldScene_SelectActorModeFromInputBit(s32 arg0)
{
    if ((*(u32 *)&gFrameCount >> 1) & 1) {
        Object_SetPalette(arg0, 10);
    } else {
        Object_SetPalette(arg0, 9);
    }
}

void FieldScene_RunParticleRain(void)
{
    struct Descriptor_020041ec descriptor;
    u8 *record;
    u32 i;
    s32 x;
    s32 y;
    s32 scale;

    Audio_PlayCue(0x83);
    *(u32 *)(((u8 *)Object_GetById(8)) + 108) = (u32)FieldScene_SelectActorModeFromInputBit;
    Event_Wait(40);
    ColorBuffer_ApplySource(128 << 9, 0);
    ColorBuffer_ApplyTarget(0x205c54, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(40);
    Audio_PlayCue(0x83);
    *(u32 *)(((u8 *)Object_GetById(2)) + 108) = (u32)FieldScene_SelectActorModeFromInputBit;
    Event_Wait(120);
    record = Actor_Get(8);
    descriptor.field0 = 1;
    descriptor.field4 = 2;
    descriptor.field24 = 0x011d;
    for (i = 0; i <= 63; i++) {
        if ((i & 3) == 0) {
            Audio_PlayCue(246);
        }
        x = *(s32 *)(record + 8)
            + ((((u32)(Random_Next() * 3) << 4) >> 16) << 16)
            + 0xfff40000;
        y = *(s32 *)(record + 12)
            + ((((u32)Random_Next() << 5) >> 16) << 16)
            + 0xfff00000;
        scale = (((u32)((u32)Random_Next() << 2) >> 16) << 15) + (128 << 8);
        Effect_Spawn(x, y, *(s32 *)(record + 16), 0,
                      scale, 0, 152 << 13, (u8 *)&descriptor);
        Task_Wait(2);
    }
    Audio_PlayCue(220);
    Event_Wait(30);
    ColorBuffer_ApplyTarget(128 << 9, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(40);
    *(u32 *)(((u8 *)Object_GetById(8)) + 108) = 0;
    *(u32 *)(((u8 *)Object_GetById(2)) + 108) = 0;
    Actor_SetChildValue(8, 0);
    Actor_SetChildValue(ACTOR_IVAN, 0);
}
