#include "GROUP_DEPARTURE.H"
#include "TYPES.H"
#include "CALL.H"

extern u8 MsgHaidiaBigBoyWhy[];
extern u8 MsgHaidiaKnowWayGo[];
extern u8 MsgHaidiaKyleAbleStop[];
void BattleFx_SetBlock30Values12Zero(void);

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_WorkSetValuesIfNonNegative();
void Engine_AudioPlayCue();
void Engine_TaskWait();
void Engine_ActorSetPosition();
void Engine_ActorWalkToAndWait();
void Engine_EventWait();
void Engine_MapWaitWorkValuesBelow256();
void BattleFx_PlayQueuedSound();
void HaidiaArashi_SetStormCellAttributes();
void SceneActor_RunActor22PlacementSequence();
void Engine_EventEnd();

struct Flags35 {
    u8 pad[35];
    u8 flags;
};

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

void ActorPresentation_SetEightSceneCells();
void ObjectMotion_SetActionVariant();

void FieldScene_DrawFiveTileBlocks();

extern u8 MsgHaidiaHuh[];

extern u8 MsgHaidiaDontLeaveMeHere[];
extern u8 MsgHaidiaUghHrnghhh[];
extern u8 MsgHaidiaBoulderNeedGet[];
extern u8 MsgHaidiaWantDumpStuff[];

extern u8 MsgHaidiaKnowRightOk[];
extern u8 MsgHaidiaRightDitchStuff[];
extern u8 MsgHaidiaRockHitsLose[];
extern u8 MsgHaidiaThinkForgetThings[];
s32 Engine_EventOpenMessage();
void Engine_ActorFaceEachOther();
s32 Engine_EventChooseYesNo();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorSetAttachedEffect();
void Engine_ActorSetAnimation();
void Engine_ActorFaceActor();
void Engine_ActorSetAnimationAndWait();
void Engine_EventShowMessage();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();
void Event_PrepareObjectAndApplyValue();

extern u8 MsgHaidiaHey[];
extern u8 HaidiaArashi_ActorTwentyTwoScriptA[];
extern u8 HaidiaArashi_ActorTwentyTwoScriptB[];

/* Configures actor 22 (position, pose, and movement/sprite flags) for the
 * scene. */
s32 OverlayObject_SetField6OnCountdown(struct Object *object)
{
    s32 loaded = object->counter;
    s32 counter = (s16)loaded;

    if (loaded == 0) {
        object->x = Random_Next();
        counter = IwramUnsignedRemainder(Random_Next(), 20) + 20;
        object->counter = counter;
    }
    object->counter = counter - 1;
    return 1;
}

s32 UpdateFixedPointCountdown(struct FixedPointCountdown *state)
{
    switch (state->countdown) {
    case 6:
        state->fixed_point_18 += (s32) 0xFFFFC000;
        state->fixed_point_1c += 0x2000;
        break;
    case 4:
        state->fixed_point_18 += 0x2000;
        state->fixed_point_1c += -0x1000;
        break;
    case 2:
        state->fixed_point_18 += 0x1000;
        state->fixed_point_1c += (s32) 0xFFFFF800;
        break;
    case 0:
        state->fixed_point_18 = 0x10000;
        state->fixed_point_1c = 0x10000;
        state->countdown = (s16)(IwramUnsignedRemainder(Random_Next(), 90) + 60);
        break;
    }
    state->countdown--;
    return 1;
}

u8 *SceneData_GetTableD0E4(void)
{
    return HaidiaArashi_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableD27c(void)
{
    return HaidiaArashi_SceneTable1;
}

u8 *SceneData_GetTableD2B8(void)
{
    return HaidiaArashi_SceneTable2;
}

u8 *SceneData_GetTableD558(void)
{
    return HaidiaArashi_SceneTable3;
}

void SceneState_SetFlag210AndConfigureRegion40_84(void)
{
    s32 a;
    s32 b;

    GameFlag_Set(0x210);
    a = 10;
    b = 84;
    Map_CopyCellAttributes(40, 84, 7, 4, a, b);
}

void SceneState_SetFlag210AndConfigureRegion40_89(void)
{

    s32 a;
    s32 b;

    GameFlag_Clear(0x210);
    a = 10;
    b = 84;
    Map_CopyCellAttributes(40, 89, 7, 4, a, b);
}

void SceneState_SetWork1c0AndRunObject(u8 *o)
{

    u8 *state;

    if (GameFlag_IsSet(0x834) != 0) {
        BattleFx_SetBlock30ValuesMaxZero();
    }
    state = (u8 *)gEventWork;
    *(s32 *)(state + 0x1C0) = 0x100;
    *(s32 *)(state + 0x1C8) = 16;
    Event_RequestExit(o);
}

void FieldScene_SetupDescriptorD774(void)
{
    Audio_PlayCue(0x9E);
    Map_AnimateCells(HaidiaArashi_CellSteps0, 45, 11);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x101, 0x1A4);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(11);
}

void SceneState_SetValue123Mode1(void)
{

    Audio_PlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(1);
}

void SceneState_ApplyValues123And3(void)
{

    Audio_PlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(3);
}

void SceneState_SetValue123Mode4(void)
{

    Audio_PlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(4);
}

void FieldScene_RunStep7BAndCheckFlags841And842(void)
{
    Audio_PlayCue(0x7B);
    if (GameFlag_IsSet(0x841) != 0
        && GameFlag_IsSet(0x842) == 0) {
        FieldScene_ConfigureActorTwentyTwoScene();
    }
    SceneState_SetWork1c0AndRunObject(2);
}

void FieldScene_SetupDescriptorD78a(void)
{
    Audio_PlayCue(0x9E);
    Map_AnimateCells(HaidiaArashi_CellSteps1, 54, 32);
    Engine_ActorWalkTo(0, 0x196, 0x2d7);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(5);
}

void FieldScene_RunScene372_02000278(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x206) == 0) {
        Audio_PlayCue(158);
        Map_AnimateCells(HaidiaArashi_CellSteps2, 45, 39);
    }
    if (GameFlag_IsSet(0x835) == 0) {
        record = GameFlag_IsSet(0x831);
        if (record != 0) {
            goto L_020002b4;
        }
        FieldScene_RunScene372SequenceA();
        GameFlag_Set(0x206);
    } else {
        L_020002b4:;
        Actor_WalkTo(ACTOR_PARTY_LEADER, 0x106, 0x325);
        Event_Wait(3);
        SceneState_SetWork1c0AndRunObject(6);
    }
}

void FieldScene_SetupDescriptorD78aIfFlag205Clear(void)
{
    if (GameFlag_IsSet(0x205) == 0) {
        Audio_PlayCue(0x9E);
        Map_AnimateCells(HaidiaArashi_CellSteps1, 50, 44);
    }
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x154, 0x378);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(7);
}

void FieldScene_SetupWithDescriptorD7A0(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps2, 49, 69);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x146, 0x466);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(8);
}

void FieldScene_SetupDescriptorD7b6(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps3, 52, 76);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x176, 0x4d6);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(9);
}

void FieldScene_RunScene372_02000398(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps1, 35, 74);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(10);
}

void FieldScene_RunScene372_020003cc(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps1, 35, 73);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(12);
}

void FieldScene_RunScene372_02000400(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps2, 38, 72);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 146, 0x49e);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(13);
}

s32 FieldScene_RunFlagGatedActorSetup(void)
{
    s32 m;
    s32 t;
    s32 m2;
    s32 h;
    s32 k;
    s32 w1 = 0x14E0000;
    s32 w2 = 0x3A40000;
    s32 w3 = 0xE00000;
    s32 w4 = 0x3680000;
    s32 w5 = 0x400000;
    s32 w6 = 0x1B00000;
    s32 w7 = 0x720000;
    s32 w8 = 0xC00000;
    s32 w9 = 0x2000;
    s32 w10 = 0xE30000;
    s32 w11 = 0x4000;
    s32 w12 = 0xF70000;
    s32 w13 = 0x4000;
    s32 w14 = 0xF30000;
    s32 w15 = 0x1900000;
    s32 w16 = 0x1A80000;
    s32 w17 = 0x190;
    s32 w18 = 0x1A8;
    s32 w19 = 0x1A80000;
    s32 w20 = 0x1A8;
    s32 b1 = 0x4BE0000;
    s32 b2 = 0x4BE0000;
    s32 b3 = 0x4BE0000;
    s32 w21 = 0xA50000;
    s32 w22 = 0xA50000;
    s32 p1 = 0x2BF0000;
    s32 p2 = 0x47B0000;
    s32 p3 = 0x14D0000;
    s32 p4 = 0x4FD0000;
    s32 p5 = 0x2630000;
    s32 p6 = 0x2730000;
    s32 p7 = 0x2730000;
    s32 c1 = 0x26B;
    s32 c2 = 0x101;
    s32 c3 = 0x26B;

    BattleFx_SetQueuedSoundAndPlay(0xAA);
    Actor_SetPosition(23, 0, 0);
    if (GameFlag_IsSet(0x109) != 0) {
        GameFlag_Clear(0x205);
        GameFlag_Clear(0x206);
    }
    if (GameFlag_IsSet(0x830) != 0) {
        Actor_SetPosition(11, w1, w2);
        HaidiaArashi_SetStormCellAttributes();
    }
    if (GameFlag_IsSet(0x831) != 0) {
        Actor_SetPosition(12, w3, w4);
        ActorPresentation_SetEightSceneCells();
    }
    if (GameFlag_IsSet(0x832) != 0) {
        Actor_SetPosition(13, w5, p1);
        SceneState_ApplyFourRects();
    }
    if (GameFlag_IsSet(0x833) != 0) {
        Actor_SetPosition(14, w6, p2);
        FieldScene_DrawFiveTileBlocks();
    }
    {
        u8 *q;
        q = Actor_Get(11);
        q += 0x59;
        m = 4;
        *q = *q | m;
        q = Actor_Get(12);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(13);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(14);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(15);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(16);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(17);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(18);
        q += 0x59;
        { u8 tmp = m | *q; *q = tmp; }
    }
    if (GameFlag_IsSet(0x837) != 0) {
        Actor_SetPosition(22, 0, 0);
    }
    {
        u8 *q;
        q = Actor_Get(19);
        *(s32 *)(q + 0x18) = 0x20000;
        *(s32 *)(q + 0x1C) = 0x20000;
    }
    if (GameFlag_IsSet(FLAG_BOULDER_FELL) != 0) {
        Actor_SetPosition(19, w7, p3);
    } else {
        Actor_EnableActionCallback(19, HaidiaArashi_ActorNineteenScript);
    }
    if (GameFlag_IsSet(0x841) != 0) {
        s32 h2;
        u8 *tbl;
        FieldScene_BuildPlacementGrid();
        Actor_SetPosition(9, w21, 0x4CD0000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(9);
            h2 = 0xE000;
            *(u16 *)(o + 6) = h2;
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            tbl = HaidiaArashi_ActorEightScript;
            o += 0x66;
            t = 1;
            *(u16 *)o = t;
            Actor_EnableActionCallback(9, tbl);
        }
        Actor_SetPosition(26, w22, 0x4E60000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(26);
            *(u16 *)(o + 6) = h2;
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 2;
            *(u16 *)o = t;
            Actor_EnableActionCallback(26, tbl);
        }
        Actor_SetPosition(22, 0x980000, 0x5050000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(22);
            *(u16 *)(o + 6) = h2;
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 3;
            *(u16 *)o = t;
            Actor_EnableActionCallback(22, tbl);
        }
        Actor_SetPosition(8, 0xB80000, 0x5180000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(8);
            *(u16 *)(o + 6) = h2;
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 4;
            *(u16 *)o = t;
            Actor_EnableActionCallback(8, tbl);
        }
        Actor_SetAnimation(8, 6);
        {
            u8 *r;
            r = Actor_Get(22);
            r += 0x23;
            m2 = 0xFE;
            *r = *r & m2;
            r = Actor_Get(8);
            r += 0x23;
            *r = m2 & *r;
        }
        Scheduler_AddOrUpdateCallback(OverlayObject_CopyRecordField1ToSlots22And8, 0xC80);
        Actor_SetPosition(24, 0, 0);
        Actor_SetPosition(25, 0, 0);
        Actor_SetPosition(23, 0, 0);
        Actor_SetPosition(19, 0, 0);
        if (GameFlag_IsSet(0x842) != 0) {
            Actor_SetPosition(22, 0, 0);
        }
    } else if (GameFlag_IsSet(0x83a) != 0) {
        u8 *tbl;
        Actor_SetPosition(10, w8, b1);
        Actor_FaceDirection(10, w9, 0);
        Actor_SetAnimation(10, 5);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(10);
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        tbl = HaidiaArashi_ActorEightScript;
        Actor_EnableActionCallback(10, tbl);
        Actor_SetPosition(24, w10, b2);
        Actor_FaceDirection(24, w11, 0);
        Actor_SetAnimation(24, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(24);
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Actor_EnableActionCallback(24, tbl);
        Actor_SetPosition(25, w12, b3);
        Actor_FaceDirection(25, w13, 0);
        Actor_SetAnimation(25, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(25);
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Actor_EnableActionCallback(25, tbl);
        Actor_SetPosition(23, w14, p4);
        Actor_FaceDirection(23, 0xC000, 0);
        Actor_SetSpriteFlags(Actor_Get(23), 0);
        Actor_SetPosition(17, 0, 0);
        Actor_SetPosition(18, 0, 0);
    } else {
        Actor_SetPosition(17, 0, 0);
        Actor_SetPosition(18, 0, 0);
    }
    {
        s16 *table = (s16 *)&gGameState;
        if (table[225] != 15 || GameFlag_IsSet(0x87b) != 0) {
            Effect_SoundAndFlash();
            BattleFx_StartTwelveFrameBlend();
        }
    }
    if (GameFlag_IsSet(0x210) != 0) {
        SceneState_SetFlag210AndConfigureRegion40_84();
    }
    GameFlag_Set(0x834);
    k = 46;
    Map_CopyCellAttributes(29, 24, 1, 2, 26, k);
    Map_CopyCellAttributes(29, 25, 1, 1, 27, k);
    Map_CopyCellAttributes(29, 25, 1, 1, 28, k);
    k = 20;
    Map_CopyCellAttributes(19, 0x5A, 1, 1, k, 0x58);
    Map_CopyCellAttributes(19, 0x5A, 1, 1, k, 0x59);
    {
        u8 *o;
        u8 *q;
        o = Actor_Get(21);
        q = o + 0x55;
        *q = 0;
        *(s32 *)(o + 0xC) = 0xC00000;
        q += 4;
        *q = 8;
        Actor_SetSpriteFlags(o, 0);
    }
    Task_Wait(1);
    if (GameFlag_IsSet(0x87b) == 0) {
        s16 *table = (s16 *)&gGameState;
        if (table[225] == 15) {
            Scene_DoraSendsRobinToThePlaza();
            return 0;
        }
    }
    Actor_SetAnimation(23, 7);
    if (GameFlag_IsSet(0x837) == 0) {
        Event_Begin();
        Actor_SetAttachedEffect(22, c2);
        Actor_SetPosition(22, w15, p5);
        Actor_SetPosition(21, w16, p6);
        Actor_WalkTo(22, w17, c1);
        Actor_WalkToAndWait(21, w18, 0x26B);
        Actor_SetAnimation(21, 2);
        Actor_SetAnimation(22, 5);
        Event_End();
    } else {
        Event_Begin();
        Actor_SetPosition(21, w19, p7);
        Actor_WalkToAndWait(21, w20, c3);
        Actor_SetAnimation(21, 3);
        Event_End();
    }
    {
        u8 *state = (u8 *)gEventWork;
        *(s32 *)(state + 0x1C0) = 0x100;
        *(s32 *)(state + 0x1C8) = 24;
    }
    Event_OpenScreen();
    Event_WaitForScreen();
    BattleFx_SetBlock30Values128One();
    return 0;
}

/* The storm night: Dora sends Robin to the plaza. She asks Kyle whether the
 * Boulder can be stopped, asks Robin to go, and repeats her plea for as long
 * as he refuses; then Kyle and Dora leave for the plaza. */
void Scene_DoraSendsRobinToThePlaza(void)
{
    s32 message;

    Event_Begin();
    Effect_SoundAndFlash();
    BattleFx_StartTwelveFrameBlend();
    BattleFx_SetBlock30Values12Zero();
    Task_Wait(60);
    Camera_SetSpeed(0x4000, 0x800);
    Camera_MoveTo(0x13c0000, 0xa00000, 0x3700000, 1);
    Actor_SetPosition(ACTOR_KYLE, 0x1260000, 0x3640000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 16;
    Event_OpenScreen();
    Event_WaitForScreen();
    BattleFx_SetBlock30Values128One();
    Audio_PlayCue(158);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps1, 50, 44);
    Actor_SetAttachedEffect(22, 0x101);
    Actor_SetSpeed(ACTOR_DORA, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_KYLE, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_DORA, 0x1560000, 0x37a0000);
    Actor_WalkToAndWait(ACTOR_DORA, 0x156, 0x389);
    BattleFx_PlayQueuedSound();
    Actor_WalkTo(ACTOR_DORA, 0x128, 0x389);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1560000, 0x37a0000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x156, 0x37a);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x156, 0x389);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x13e, 0x389);
    Actor_SetAnimation(ACTOR_DORA, 1);
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    Actor_FaceDirection(ACTOR_DORA, 0xc000, 60);
    message = (s32)MsgHaidiaKyleAbleStop;
    Event_SetMessage(message);
    Event_ShowMessage(ACTOR_DORA, 0);
    Actor_WalkToAndWait(ACTOR_KYLE, 0x126, 0x346);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_KYLE, 4);
    Event_ShowMessage(ACTOR_KYLE, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_KYLE, 0x4000, 0);
    Event_ShowMessageAndWait(ACTOR_KYLE, 0, 20);
    Actor_ShowEmote(ACTOR_DORA, 0x101, 20);
    Actor_FaceDirection(ACTOR_DORA, 0xc000, 10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_KYLE, 4);
    Event_ShowMessage(ACTOR_KYLE, 0);
    Actor_SetAttachedEffect(ACTOR_DORA, 0x102);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_DORA, 0, 50);
    Actor_FaceDirection(ACTOR_DORA, 0xc000, 10);
    Actor_SetSpeed(ACTOR_DORA, 0x18000, 0xc000);
    Actor_WalkToAndWait(ACTOR_DORA, 0x121, 0x373);
    Actor_FaceDirection(ACTOR_DORA, 0xe000, 0);
    Event_ShowMessage(ACTOR_DORA, 0);
    Actor_RunRepeatedMotion(ACTOR_KYLE, 2);
    Event_ShowMessage(ACTOR_KYLE, 0);
    Actor_SetAnimationAndWait(ACTOR_DORA, 4);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_FaceDirection(ACTOR_DORA, 0x2000, 10);
    Event_SetMessage((message + 8));
    Event_OpenMessage(ACTOR_DORA, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x12e, 0x389);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    while (Event_ChooseYesNo(0, 0) == 1) {
        Actor_RunRepeatedMotion(ACTOR_DORA, 1);
        Event_SetMessage((s32)MsgHaidiaBigBoyWhy);
        Event_OpenMessage(ACTOR_DORA, 0);
    }
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Event_SetMessage((s32)MsgHaidiaKnowWayGo);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetSpeed(ACTOR_KYLE, 0x18000, 0xc000);
    Actor_WalkTo(ACTOR_KYLE, 0x129, 0x2ee);
    Event_Wait(10);
    Actor_WalkToAndWait(ACTOR_DORA, 0x129, 0x2ee);
    Actor_SetPosition(ACTOR_DORA, 0, 0);
    Actor_SetPosition(ACTOR_KYLE, 0, 0);
    Actor_SetAnimation(ACTOR_KYLE, 1);
    Actor_SetAnimation(21, 2);
    Actor_SetAnimation(22, 5);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Set(0x87b);
    GameFlag_Set(0x205);
    Event_End();
}

void HaidiaArashi_RunScene00D5C(void)
{
    u32 i;
    u8 *rec7;
    s32 record;
    s32 v3;
    s32 base5_396;
    u8 *p6;

    if (Value1(Engine_GameFlagIsSet, 0x310) != 0) {
    } else {
        Engine_EventBegin();
        if (Value1(Engine_GameFlagIsSet, 0x830) == 0) {
            rec7 = (u8 *)Engine_ActorGet(11);
            p6 = *(s32 *)((s32)rec7 + 80);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
            Engine_AudioPlayCue(141);
            Engine_TaskWait(40);
            Engine_AudioPlayCue(145);
            ((struct Flags35 *)rec7)->flags &= 254;
            ((struct Flags9 *)p6)->mode = 1;
            Call3(Engine_ActorSetPosition, 11, 0x1d90000, 0x3a40000);
            *(s32 *)((s32)rec7 + 48) = 0x18000;
            *(s32 *)((s32)rec7 + 52) = 0x18000;
            v3 = (*(s32 *)((s32)rec7 + 12) + 0xf00000);
            *(s32 *)((s32)rec7 + 12) += 0xf00000;
            *(s32 *)((s32)rec7 + 60) = v3;
            *(s32 *)((s32)rec7 + 68) = 0x6666;
            Call3(Engine_ActorWalkToAndWait, 11, 0x158, 0x3a4);
            ((struct Flags9 *)p6)->mode = 3;
            ((struct Flags35 *)rec7)->flags |= 1;
            Engine_EventWait(40);
            Engine_AudioPlayCue(0x121);
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            BattleFx_PlayQueuedSound();
            Engine_GameFlagSet(0x830);
        }
        HaidiaArashi_SetStormCellAttributes();
        Engine_GameFlagSet(0x310);
        if (Engine_GameFlagIsSet(0x837) != 0) {
            if (Engine_GameFlagIsSet(0x841) == 0) {
                if (Engine_GameFlagIsSet(0x30c) == 0) {
                    record = (u8 *)Engine_ActorGet(0);
                    if (*(s32 *)(record + 12) > 0x800000) {
                        base5_396 = 0x396;
                        SceneActor_RunActor22PlacementSequence(0x146, base5_396);
                        Call3(Engine_ActorWalkToAndWait, 0, 0x123, base5_396);
                    } else {
                        SceneActor_RunActor22PlacementSequence(0x14f, 0x3bd);
                    }
                    Engine_GameFlagSet(0x30c);
                }
            }
        }
        Engine_EventEnd();
    }
}

void HaidiaArashi_SetStormCellAttributes(void)
{
    u32 i;
    s32 record;

    Map_CopyCellAttributes(29, 64, 1, 1, 21, 57);
    Map_CopyCellAttributes(29, 64, 1, 1, 21, 58);
    Map_CopyCellAttributes(29, 64, 1, 1, 22, 58);
    Map_CopyCellAttributes(29, 64, 1, 1, 20, 58);
    Map_CopyCellAttributes(28, 20, 1, 1, 20, 57);
}

void HaidiaArashi_RunEventSequence(void)
{
    s32 rec7;
    s32 record;
    s32 v3;

    if (Value1(Engine_GameFlagIsSet, 0x311) != 0) {
    } else {
        Engine_EventBegin();
        if (Value1(Engine_GameFlagIsSet, 0x831) == 0) {
            rec7 = (s32)Engine_ActorGet(12);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
            Engine_AudioPlayCue(141);
            Engine_TaskWait(40);
            Engine_AudioPlayCue(145);
            Call3(Engine_ActorSetPosition, 12, 0x17d0000, 0x3280000);
            *(s32 *)(rec7 + 48) = 0x18000;
            *(s32 *)(rec7 + 52) = 0x18000;
            v3 = (*(s32 *)(rec7 + 12) + 0x1000000);
            *(s32 *)(rec7 + 12) += 0x1000000;
            *(s32 *)(rec7 + 60) = v3;
            *(s32 *)(rec7 + 68) = 0x8000;
            Call3(Engine_ActorWalkToAndWait, 12, 0x122, 0x341);
            ObjectMotion_SetActionVariant(12, 1);
            Call3(Engine_ActorWalkToAndWait, 12, 0x102, 0x354);
            ObjectMotion_SetActionVariant(12, 2);
            Call3(Engine_ActorWalkToAndWait, 12, 224, 0x368);
            Engine_EventWait(40);
            Engine_AudioPlayCue(0x121);
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            BattleFx_PlayQueuedSound();
            Engine_GameFlagSet(0x831);
        }
        ActorPresentation_SetEightSceneCells();
        Engine_GameFlagSet(0x311);
        if (Engine_GameFlagIsSet(0x837) != 0) {
            if (Engine_GameFlagIsSet(0x841) == 0) {
                if (Engine_GameFlagIsSet(0x30c) == 0) {
                    record = (s32)Engine_ActorGet(0);
                    if (*(s32 *)(record + 12) > 0x800000) {
                        SceneActor_RunActor22PlacementSequence(219, 0x34b);
                        Call3(Engine_ActorWalkToAndWait, 0, 179, 0x33d);
                    } else {
                        SceneActor_RunActor22PlacementSequence(214, 0x38c);
                        Call3(Engine_ActorWalkToAndWait, 0, 219, 0x38f);
                    }
                    Engine_GameFlagSet(0x30c);
                }
            }
        }
        Engine_EventEnd();
    }
}

void ActorPresentation_SetEightSceneCells(void)
{
    s32 a = 15;
    s32 d = 0x35;
    s32 e;
    s32 b;
    s32 c;
    s32 f;

    Map_CopyCellAttributes(29, 23, 1, 1, a, d);
    b = 14;
    Map_CopyCellAttributes(29, 23, 1, 1, b, d);
    c = 13;
    Map_CopyCellAttributes(29, 23, 1, 1, c, d);
    Map_CopyCellAttributes(26, 20, 2, 1, b, 0x34);
    e = 0x36;
    Map_CopyCellAttributes(25, 21, 1, 1, c, e);
    Map_CopyCellAttributes(25, 21, 1, 1, a, e);
    Map_CopyCellAttributes(14, 0x35, 1, 1, b, e);
    f = 0x37;
    Map_CopyCellAttributes(13, 0x37, 1, 1, a, f);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    if (GameFlag_IsSet(0x312) == 0) {
        Event_Begin();
        if (GameFlag_IsSet(0x832) == 0) {
            struct FieldActor *actor = Actor_Get(13);
            struct FieldActor *leader = Actor_Get(ACTOR_PARTY_LEADER);
            u16 priority = leader->sprite->priority;
            u8 flags = leader->priority_flags;

            Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
            Audio_PlayCue(141);
            Task_Wait(40);
            Audio_PlayCue(145);
            ObjectMotion_SetActionVariant(ACTOR_PARTY_LEADER, 3);
            Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            Actor_SetPosition(13, 0, 0x2bf0000);
            actor->speed = 0x18000;
            actor->acceleration = 0x18000;
            actor->y.fixed += 0x500000;
            *(s32 *)((u8 *)actor + 0x3c) = actor->y.fixed;
            *(s32 *)((u8 *)actor + 0x44) = 0x8000;
            Actor_WalkToAndWait(13, 64, 0x2bf);
            Event_Wait(40);
            Audio_PlayCue(0x121);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            BattleFx_PlayQueuedSound();
            GameFlag_Set(0x832);
            ObjectMotion_SetActionVariant(ACTOR_PARTY_LEADER, priority);
            Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
            leader->priority_flags = flags;
        }
        SceneState_ApplyFourRects();
        GameFlag_Set(0x312);
        if (GameFlag_IsSet(0x837) != 0) {
            if (GameFlag_IsSet(0x841) == 0) {
                if (GameFlag_IsSet(0x30c) == 0) {
                    if (Actor_Get(ACTOR_PARTY_LEADER)->z.fixed <= 0x2b4ffff) {
                        SceneActor_RunActor22PlacementSequence(62, 0x29d);
                        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 27, 0x273);
                    } else {
                        SceneActor_RunActor22PlacementSequence(75, 0x2cb);
                        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 67, 0x2f5);
                    }
                    GameFlag_Set(0x30c);
                }
            }
        }
        Event_End();
    }
}

void SceneState_ApplyFourRects(void)
{

    s32 a = 0x2a;
    s32 b;

    Map_CopyCellAttributes(29, 22, 1, 1, 3, a);
    b = 2;
    Map_CopyCellAttributes(29, 21, 1, 1, b, a);
    Map_CopyCellAttributes(29, 21, 1, 1, 4, a);
    Map_CopyCellAttributes(23, 20, 3, 1, b, 0x2b);
}

void HaidiaArashi_RunSecondEventSequence(void)
{
    s32 rec7;
    s32 record;
    s32 v3;

    if (Value1(Engine_GameFlagIsSet, 0x313) != 0) {
    } else {
        Engine_EventBegin();
        if (Value1(Engine_GameFlagIsSet, 0x833) == 0) {
            rec7 = (s32)Engine_ActorGet(14);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
            Engine_AudioPlayCue(141);
            Engine_TaskWait(40);
            Engine_AudioPlayCue(145);
            Call3(Engine_ActorSetPosition, 14, 0x1da0000, 0x47b0000);
            *(s32 *)(rec7 + 48) = 0x10000;
            *(s32 *)(rec7 + 52) = 0x10000;
            v3 = (*(s32 *)(rec7 + 12) + 0x480000);
            *(s32 *)(rec7 + 12) += 0x480000;
            *(s32 *)(rec7 + 60) = v3;
            *(s32 *)(rec7 + 68) = 0x8000;
            Engine_ActorWalkToAndWait(14, 0x1b0, 0x47b);
            Engine_EventWait(40);
            Engine_AudioPlayCue(0x121);
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            BattleFx_PlayQueuedSound();
            Engine_GameFlagSet(0x833);
        }
        FieldScene_DrawFiveTileBlocks();
        Engine_GameFlagSet(0x313);
        if (Engine_GameFlagIsSet(0x837) != 0) {
            if (Engine_GameFlagIsSet(0x841) == 0) {
                if (Engine_GameFlagIsSet(0x30c) == 0) {
                    record = (s32)Engine_ActorGet(0);
                    if (*(s32 *)(record + 16) <= 0x479ffff) {
                        SceneActor_RunActor22PlacementSequence(0x19c, 0x460);
                        Call3(Engine_ActorWalkToAndWait, 0, 0x19e, 0x42c);
                    } else {
                        SceneActor_RunActor22PlacementSequence(0x1bd, 0x494);
                        Call3(Engine_ActorWalkToAndWait, 0, 0x1bf, 0x4cb);
                    }
                    Engine_GameFlagSet(0x30c);
                }
            }
        }
        Engine_EventEnd();
    }
}

void FieldScene_DrawFiveTileBlocks(void)
{
    s32 a = 26;
    s32 h = 0x47;
    s32 b;
    s32 a2;

    Map_CopyCellAttributes(29, 20, 1, 1, a, h);
    b = 0x46;
    Map_CopyCellAttributes(29, 20, 1, 1, a, b);
    a2 = 27;
    Map_CopyCellAttributes(29, 20, 1, 1, a2, b);
    Map_CopyCellAttributes(28, 21, 1, 1, 28, h);
    Map_CopyCellAttributes(28, 22, 1, 1, a2, 0x48);
}

void FieldScene_RunScene372SequenceA(void)
{
    s32 base;

    Event_Begin();
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x106, 0x32a);
    Actor_SetPosition(20, 0x1060000, 0x3250000);
    Actor_WalkTo(20, 0x106, 0x339);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x11a, 0x357);
    Actor_SetAnimation(20, 1);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 20, 0);
    BattleFx_PlayQueuedSound();
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_ShowEmote(20, 0x100, 20);
    base = (s32)MsgHaidiaHuh;
    Event_SetMessage(base);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Event_AskYesNo(20, 0);
    Actor_RunRepeatedMotion(20, 2);
    Event_SetMessage((base + 4));
    Event_ShowMessageAndWait(20, 0, 20);
    Object_SetActionCallbackAndRefreshById(20, (s32)HaidiaArashi_ActorTwentyScript);
    GameFlag_Set(0x835);
    Event_End();
}

void FieldScene_RunScene372SequenceB(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x836) == 0) {
        if (GameFlag_IsSet(0x837) == 0) {
            Event_Begin();
            Event_SetMessage((s32)MsgHaidiaUghHrnghhh);
            Event_ShowMessageAndWait(22, 0, 20);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 40);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x17e, 0x26b);
            Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
            Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Event_Wait(30);
            Event_ShowMessage(22, 0);
            GameFlag_Set(0x836);
            Event_End();
        }
    }
}

void FieldScene_RunActor22SceneWhenFlag836Only(void)
{
    if (GameFlag_IsSet(0x837) == 0 && GameFlag_IsSet(0x836) != 0) {
        Event_Begin();
        Actor_RunRepeatedMotion(22, 2);
        Event_Wait(20);
        Event_SetMessage((s32)MsgHaidiaDontLeaveMeHere);
        HaidiaArashi_RunCallOutSequence();
        Event_End();
    }
}

void FieldScene_RunScene372SequenceC(void)
{
    if (GameFlag_IsSet(0x841) != 0) {
        Event_Begin();
        Actor_FaceActor(22, ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Event_SetMessage((s32)MsgHaidiaBoulderNeedGet);
        Event_ShowMessage(22, 0);
        Actor_FaceDirection(22, 0xe000, 10);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x837) == 0) {
            Event_Begin();
            Event_SetMessage((s32)MsgHaidiaWantDumpStuff);
            HaidiaArashi_RunCallOutSequence();
            Event_End();
        }
    }
}

void HaidiaArashi_RunCallOutSequence(void)
{
    s32 record;
    s32 v5;

    Engine_EventOpenMessage(22, 0);
    Engine_ActorFaceEachOther(0, 22, 0);
    v5 = 0;
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventSetMessage((s32)MsgHaidiaThinkForgetThings);
        v5 = 1;
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaRockHitsLose);
    }
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(22, 0, 40);
    Call2(Engine_ActorSetAttachedEffect, 22, 0x100);
    Engine_ActorSetAnimation(21, 3);
    Engine_ActorSetAnimation(22, 1);
    Engine_EventWait(40);
    Engine_ActorFaceActor(22, 0, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(22, 3);
    if (v5 != 0) {

        Engine_EventSetMessage((s32)MsgHaidiaKnowRightOk);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaRightDitchStuff);
    }
    Engine_EventShowMessage(22, 0);
    Engine_ActorSetAnimation(22, 2);
    record = (s32)Engine_ActorGet(0);
    if (record != 0) {
        Engine_ActorSetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(22);
    Engine_ActorSetPosition(22, 0, 0);
    Event_PrepareObjectAndApplyValue(1, 1);
    Engine_GameFlagSet(0x837);
}

void FieldScene_RunScene372SequenceE(void)
{
    s32 record;
    s32 base;

    if (GameFlag_IsSet(0x837) == 0) {
        Event_Begin();
        Actor_SetAttachedEffect(22, 0x100);
        base = (s32)MsgHaidiaHey;
        Event_SetMessage(base);
        Event_ShowMessage(22, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Camera_SetSpeed(0x6666, 0xccc);
        Camera_MoveTo(0x1000000, -1, 0x24c0000, 1);
        Actor_SetSpeed(22, 0x20000, 0x10000);
        ((s32 (*)())Object_SetActionCallbackAndRefreshById)(22, (s32)HaidiaArashi_ActorTwentyTwoScriptA);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
        Event_Wait(30);
        ((s32 (*)())Engine_ActorEnableActionCallback)(22, (s32)HaidiaArashi_ActorTwentyTwoScriptB);
        Event_ShowMessage(22, 0);
        record = Actor_Get(22);
        *(s32 *)(record + 28) = 0x10000;
        Actor_RunRepeatedMotion(22, 1);
        Event_Wait(20);
        Event_AskYesNo(22, 0);
        Event_Wait(40);
        Actor_RunRepeatedMotion(22, 1);
        Event_SetMessage((base + 5));
        Event_ShowMessageAndWait(22, 0, 20);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimationAndWait(22, 3);
        Event_ShowMessage(22, 0);
        Actor_SetSpeed(22, 0x10000, 0x8000);
        Actor_SetAnimation(22, 2);
        record = Engine_ActorGet(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(22);
        Actor_SetPosition(22, 0, 0);
        Event_PrepareObjectAndApplyValue(1, 1);
        Actor_SetAnimation(21, 3);
        GameFlag_Set(0x837);
        Event_End();
    }
}
