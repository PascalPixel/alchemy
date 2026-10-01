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

extern u8 MsgHaidiaCantGetAroundThisRock[];
extern u8 MsgHaidiaNorthLeadsToMtAleph[];
extern u8 MsgHaidiaTheBoulderIsFalling[];

/*
 * The overlay's own work, after its image: whether the boulder has stopped
 * shaking, and the shift that slows the shaking.
 */
s32 HaidiaArashi_ShakeDone;
s32 HaidiaArashi_ShakeShift;

extern u8 MsgHaidiaNoBrother[];
void HaidiaArashi_RunRiverSearch(void);
extern u8 MsgHaidiaDontSupposeTwo[];
extern u8 MsgHaidiaGoLookNorth[];
extern u8 HaidiaArashi_ActorNineScriptA[];
extern u8 HaidiaArashi_ActorNineScriptB[];
extern u8 HaidiaArashi_ActorNineScriptC[];
extern u8 HaidiaArashi_ActorNineScriptD[];
extern u8 HaidiaArashi_ActorTwentySixScriptA[];
extern u8 HaidiaArashi_ActorTwentySixScriptB[];
extern u8 HaidiaArashi_ActorTwentySixScriptC[];
extern u8 HaidiaArashi_CellSteps4[];
extern u8 HaidiaArashi_CellSteps5[];
extern u8 MsgHaidiaOh[];
extern u8 MsgHaidiaTwoDontEnough[];
extern u8 MsgHaidiaMomDadBack[];
void Engine_ActorFaceDirection();
void Engine_ActorRunRepeatedMotion();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_ActorEnableActionCallback();

/* Actor 9's action table for the run through the storm. */
extern u8 HaidiaArashi_StormRunActions[];
void Engine_CameraWaitForMove();
void Scene_RunActorGroupDepartureSequence();

/* Configures actor 22 (position, pose, and movement/sprite flags) for the
 * scene. */
s32 OverlayObject_SetField6OnCountdown(struct Object *object)
{
    s32 loaded = object->counter;
    s32 counter = (s16)loaded;

    if (loaded == 0) {
        object->x = Random_Next();
        counter = Math_RemainderUnsigned(Random_Next(), 20) + 20;
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
        state->countdown = (s16)(Math_RemainderUnsigned(Random_Next(), 90) + 60);
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
    Engine_EventRequestExit(o);
}

void FieldScene_SetupDescriptorD774(void)
{
    Audio_PlayCue(0x9E);
    Map_AnimateCells(HaidiaArashi_CellSteps0, 45, 11);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x101, 0x1A4);
    Engine_EventWait(3);
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
    Engine_EventWait(3);
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
        Engine_EventWait(3);
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
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(7);
}

void FieldScene_SetupWithDescriptorD7A0(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps2, 49, 69);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x146, 0x466);
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(8);
}

void FieldScene_SetupDescriptorD7b6(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps3, 52, 76);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x176, 0x4d6);
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(9);
}

void FieldScene_RunScene372_02000398(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps1, 35, 74);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(10);
}

void FieldScene_RunScene372_020003cc(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps1, 35, 73);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(12);
}

void FieldScene_RunScene372_02000400(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps2, 38, 72);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 146, 0x49e);
    Engine_EventWait(3);
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
        Engine_ActorEnableActionCallback(19, HaidiaArashi_ActorNineteenScript);
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
            v = Math_RemainderUnsigned(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            tbl = HaidiaArashi_ActorEightScript;
            o += 0x66;
            t = 1;
            *(u16 *)o = t;
            Engine_ActorEnableActionCallback(9, tbl);
        }
        Actor_SetPosition(26, w22, 0x4E60000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(26);
            *(u16 *)(o + 6) = h2;
            v = Math_RemainderUnsigned(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 2;
            *(u16 *)o = t;
            Engine_ActorEnableActionCallback(26, tbl);
        }
        Actor_SetPosition(22, 0x980000, 0x5050000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(22);
            *(u16 *)(o + 6) = h2;
            v = Math_RemainderUnsigned(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 3;
            *(u16 *)o = t;
            Engine_ActorEnableActionCallback(22, tbl);
        }
        Actor_SetPosition(8, 0xB80000, 0x5180000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(8);
            *(u16 *)(o + 6) = h2;
            v = Math_RemainderUnsigned(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 4;
            *(u16 *)o = t;
            Engine_ActorEnableActionCallback(8, tbl);
        }
        Engine_ActorSetAnimation(8, 6);
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
        Engine_ActorSetAnimation(10, 5);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(10);
            v = Math_RemainderUnsigned(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        tbl = HaidiaArashi_ActorEightScript;
        Engine_ActorEnableActionCallback(10, tbl);
        Actor_SetPosition(24, w10, b2);
        Actor_FaceDirection(24, w11, 0);
        Engine_ActorSetAnimation(24, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(24);
            v = Math_RemainderUnsigned(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Engine_ActorEnableActionCallback(24, tbl);
        Actor_SetPosition(25, w12, b3);
        Actor_FaceDirection(25, w13, 0);
        Engine_ActorSetAnimation(25, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(25);
            v = Math_RemainderUnsigned(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Engine_ActorEnableActionCallback(25, tbl);
        Actor_SetPosition(23, w14, p4);
        Actor_FaceDirection(23, 0xC000, 0);
        Engine_ActorSetSpriteFlags(Actor_Get(23), 0);
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
        Engine_ActorSetSpriteFlags(o, 0);
    }
    Engine_TaskWait(1);
    if (GameFlag_IsSet(0x87b) == 0) {
        s16 *table = (s16 *)&gGameState;
        if (table[225] == 15) {
            Scene_DoraSendsRobinToThePlaza();
            return 0;
        }
    }
    Engine_ActorSetAnimation(23, 7);
    if (GameFlag_IsSet(0x837) == 0) {
        Engine_EventBegin();
        Actor_SetAttachedEffect(22, c2);
        Actor_SetPosition(22, w15, p5);
        Actor_SetPosition(21, w16, p6);
        Actor_WalkTo(22, w17, c1);
        Actor_WalkToAndWait(21, w18, 0x26B);
        Engine_ActorSetAnimation(21, 2);
        Engine_ActorSetAnimation(22, 5);
        Engine_EventEnd();
    } else {
        Engine_EventBegin();
        Actor_SetPosition(21, w19, p7);
        Actor_WalkToAndWait(21, w20, c3);
        Engine_ActorSetAnimation(21, 3);
        Engine_EventEnd();
    }
    {
        u8 *state = (u8 *)gEventWork;
        *(s32 *)(state + 0x1C0) = 0x100;
        *(s32 *)(state + 0x1C8) = 24;
    }
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    BattleFx_SetBlock30Values128One();
    return 0;
}

/* The storm night: Dora sends Robin to the plaza. She asks Kyle whether the
 * Boulder can be stopped, asks Robin to go, and repeats her plea for as long
 * as he refuses; then Kyle and Dora leave for the plaza. */
void Scene_DoraSendsRobinToThePlaza(void)
{
    s32 message;

    Engine_EventBegin();
    Effect_SoundAndFlash();
    BattleFx_StartTwelveFrameBlend();
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(60);
    Camera_SetSpeed(0x4000, 0x800);
    Camera_MoveTo(0x13c0000, 0xa00000, 0x3700000, 1);
    Actor_SetPosition(ACTOR_KYLE, 0x1260000, 0x3640000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 16;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
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
    Engine_ActorSetAnimation(ACTOR_DORA, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Actor_FaceDirection(ACTOR_DORA, 0xc000, 60);
    message = (s32)MsgHaidiaKyleAbleStop;
    Engine_EventSetMessage(message);
    Event_ShowMessage(ACTOR_DORA, 0);
    Actor_WalkToAndWait(ACTOR_KYLE, 0x126, 0x346);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_KYLE, 4);
    Event_ShowMessage(ACTOR_KYLE, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_KYLE, 0x4000, 0);
    Event_ShowMessageAndWait(ACTOR_KYLE, 0, 20);
    Actor_ShowEmote(ACTOR_DORA, 0x101, 20);
    Actor_FaceDirection(ACTOR_DORA, 0xc000, 10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_KYLE, 4);
    Event_ShowMessage(ACTOR_KYLE, 0);
    Actor_SetAttachedEffect(ACTOR_DORA, 0x102);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_DORA, 0, 50);
    Actor_FaceDirection(ACTOR_DORA, 0xc000, 10);
    Actor_SetSpeed(ACTOR_DORA, 0x18000, 0xc000);
    Actor_WalkToAndWait(ACTOR_DORA, 0x121, 0x373);
    Actor_FaceDirection(ACTOR_DORA, 0xe000, 0);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_KYLE, 2);
    Event_ShowMessage(ACTOR_KYLE, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_FaceDirection(ACTOR_DORA, 0x2000, 10);
    Engine_EventSetMessage((message + 8));
    Event_OpenMessage(ACTOR_DORA, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x12e, 0x389);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    while (Engine_EventChooseYesNo(0, 0) == 1) {
        Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
        Engine_EventSetMessage((s32)MsgHaidiaBigBoyWhy);
        Event_OpenMessage(ACTOR_DORA, 0);
    }
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Engine_EventSetMessage((s32)MsgHaidiaKnowWayGo);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetSpeed(ACTOR_KYLE, 0x18000, 0xc000);
    Actor_WalkTo(ACTOR_KYLE, 0x129, 0x2ee);
    Engine_EventWait(10);
    Actor_WalkToAndWait(ACTOR_DORA, 0x129, 0x2ee);
    Actor_SetPosition(ACTOR_DORA, 0, 0);
    Actor_SetPosition(ACTOR_KYLE, 0, 0);
    Engine_ActorSetAnimation(ACTOR_KYLE, 1);
    Engine_ActorSetAnimation(21, 2);
    Engine_ActorSetAnimation(22, 5);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Set(0x87b);
    GameFlag_Set(0x205);
    Engine_EventEnd();
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
            rec7 = (u8 *)Object_GetById(11);
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
                    record = (u8 *)Object_GetById(0);
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
            rec7 = (s32)Object_GetById(12);
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
                    record = (s32)Object_GetById(0);
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
        Engine_EventBegin();
        if (GameFlag_IsSet(0x832) == 0) {
            struct FieldActor *actor = Actor_Get(13);
            struct FieldActor *leader = Actor_Get(ACTOR_PARTY_LEADER);
            u16 priority = leader->sprite->priority;
            u8 flags = leader->priority_flags;

            Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
            Audio_PlayCue(141);
            Engine_TaskWait(40);
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
            Engine_EventWait(40);
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
        Engine_EventEnd();
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
            rec7 = (s32)Object_GetById(14);
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
                    record = (s32)Object_GetById(0);
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

    Engine_EventBegin();
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x106, 0x32a);
    Actor_SetPosition(20, 0x1060000, 0x3250000);
    Actor_WalkTo(20, 0x106, 0x339);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x11a, 0x357);
    Engine_ActorSetAnimation(20, 1);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 20, 0);
    BattleFx_PlayQueuedSound();
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_ShowEmote(20, 0x100, 20);
    base = (s32)MsgHaidiaHuh;
    Engine_EventSetMessage(base);
    Event_ShowMessage(20, 0);
    Engine_EventWait(20);
    Event_AskYesNo(20, 0);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventSetMessage((base + 4));
    Event_ShowMessageAndWait(20, 0, 20);
    Object_SetActionCallbackAndRefreshById(20, (s32)HaidiaArashi_ActorTwentyScript);
    GameFlag_Set(0x835);
    Engine_EventEnd();
}

void FieldScene_RunScene372SequenceB(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x836) == 0) {
        if (GameFlag_IsSet(0x837) == 0) {
            Engine_EventBegin();
            Engine_EventSetMessage((s32)MsgHaidiaUghHrnghhh);
            Event_ShowMessageAndWait(22, 0, 20);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 40);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x17e, 0x26b);
            Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
            Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Engine_EventWait(30);
            Event_ShowMessage(22, 0);
            GameFlag_Set(0x836);
            Engine_EventEnd();
        }
    }
}

void FieldScene_RunActor22SceneWhenFlag836Only(void)
{
    if (GameFlag_IsSet(0x837) == 0 && GameFlag_IsSet(0x836) != 0) {
        Engine_EventBegin();
        Engine_ActorRunRepeatedMotion(22, 2);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgHaidiaDontLeaveMeHere);
        HaidiaArashi_RunCallOutSequence();
        Engine_EventEnd();
    }
}

void FieldScene_RunScene372SequenceC(void)
{
    if (GameFlag_IsSet(0x841) != 0) {
        Engine_EventBegin();
        Actor_FaceActor(22, ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgHaidiaBoulderNeedGet);
        Event_ShowMessage(22, 0);
        Actor_FaceDirection(22, 0xe000, 10);
        Engine_EventEnd();
    } else {
        if (GameFlag_IsSet(0x837) == 0) {
            Engine_EventBegin();
            Engine_EventSetMessage((s32)MsgHaidiaWantDumpStuff);
            HaidiaArashi_RunCallOutSequence();
            Engine_EventEnd();
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
    record = (s32)Object_GetById(0);
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
        Engine_EventBegin();
        Actor_SetAttachedEffect(22, 0x100);
        base = (s32)MsgHaidiaHey;
        Engine_EventSetMessage(base);
        Event_ShowMessage(22, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Camera_SetSpeed(0x6666, 0xccc);
        Camera_MoveTo(0x1000000, -1, 0x24c0000, 1);
        Actor_SetSpeed(22, 0x20000, 0x10000);
        Object_SetActionCallbackAndRefreshById(22, (s32)HaidiaArashi_ActorTwentyTwoScriptA);
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
        Engine_EventWait(30);
        ((s32 (*)())Engine_ActorEnableActionCallback)(22, (s32)HaidiaArashi_ActorTwentyTwoScriptB);
        Event_ShowMessage(22, 0);
        record = Actor_Get(22);
        *(s32 *)(record + 28) = 0x10000;
        Engine_ActorRunRepeatedMotion(22, 1);
        Engine_EventWait(20);
        Event_AskYesNo(22, 0);
        Engine_EventWait(40);
        Engine_ActorRunRepeatedMotion(22, 1);
        Engine_EventSetMessage((base + 5));
        Event_ShowMessageAndWait(22, 0, 20);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimationAndWait(22, 3);
        Event_ShowMessage(22, 0);
        Actor_SetSpeed(22, 0x10000, 0x8000);
        Engine_ActorSetAnimation(22, 2);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(22);
        Actor_SetPosition(22, 0, 0);
        Event_PrepareObjectAndApplyValue(1, 1);
        Engine_ActorSetAnimation(21, 3);
        GameFlag_Set(0x837);
        Engine_EventEnd();
    }
}

void FieldScene_RunScene372SequenceD(void)
{
    u32 i;
    s32 record;
    struct FieldActor *actor;

    Engine_EventBegin();
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Actor_SetPosition(22, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(22, 0x10000, 0x8000);
    Actor_WalkToAndWait(22, 0x119, 0x1fb);
    Engine_ActorFaceEachOther(22, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(30);
    Engine_EventSetMessage((s32)MsgHaidiaNorthLeadsToMtAleph);
    Event_ShowMessage(22, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(22, 0x4000, 0);
    Event_ShowMessage(22, 0);
    Engine_ActorSetAnimation(22, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(22);
    Actor_SetPosition(22, 0, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x205);
    Engine_EventEnd();
}

void SceneActor_RunActor22PlacementSequence(s32 x, s32 y)
{
    Thing1 *a;
    s32 w = 0x10000;
    s32 h = 0x8000;
    Thing2 *b;

    a = Actor_Get(ACTOR_PARTY_LEADER);
    if (a != 0) {
        Actor_SetPosition(22, a->unk8, a->unk10);
    }
    Actor_SetSpeed(22, w, h);
    Actor_WalkToAndWait(22, x, y);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_EventWait(40);
    Engine_EventSetMessage((s32)MsgHaidiaCantGetAroundThisRock);
    Event_ShowMessage(22, 0);
    Engine_ActorRunRepeatedMotion(22, 2);
    Event_ShowMessage(22, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(22, 2);
    b = Actor_Get(ACTOR_PARTY_LEADER);
    if (b != 0) {
        Actor_SetDestination(22, b->unkA, b->unk12);
    }
    Engine_ActorWaitForMove(22);
    Actor_SetPosition(22, 0, 0);
}

void Scene_BoulderFalls(void)
{
    u32 i;
    s32 record;
    s32 *phase;
    struct FieldActor *actor;
    s32 steps;
    s32 callback;
    s32 shifted;

    if (GameFlag_IsSet(FLAG_BOULDER_FELL) == 0) {
        Engine_EventBegin();
        Event_CallWithLastActiveObjectId((s32)HaidiaArashi_LastObjectCall);
        SceneState_SetValue140Mode0();
        Engine_TaskWait(1);
        Audio_PlayCue(141);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Engine_EventWait(30);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Audio_PlayCue(145);
        Engine_EventWait(30);
        actor = Actor_Get(ACTOR_PARTY_LEADER);
        if (actor != NULL) {
            Actor_SetPosition(22, actor->x.fixed, actor->z.fixed);
        }
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
        Actor_SetSpeed(22, 0x20000, 0x10000);
        Engine_ActorEnableActionCallback(0, (s32)HaidiaArashi_LeaderScript);
        Object_SetActionCallbackAndRefreshById(22, (s32)HaidiaArashi_ActorTwentyTwoScript);
        Object_RefreshSelectorById(0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
        Actor_ShowEmote(22, 0x100, 30);
        Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Audio_PlayCue(145);
        Engine_EventWait(40);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Audio_PlayCue(145);
        Engine_EventWait(20);
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAttachedEffect(22, 0x102);
        Engine_EventWait(40);
        Engine_ActorSetAnimation(32, 5);
        Engine_ActorSetAnimation(33, 5);
        Engine_ActorSetAnimation(30, 8);
        Engine_ActorSetAnimation(29, 8);
        Actor_Get(30)->scale_x = -0x10000;
        ObjectMotion_SetActionVariant(32, 2);
        ObjectMotion_SetActionVariant(33, 2);
        ObjectMotion_SetActionVariant(30, 3);
        ObjectMotion_SetActionVariant(29, 3);
        Engine_EventSetMessage((s32)MsgHaidiaTheBoulderIsFalling);
        Event_ShowMessageAndWait(28, 0, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(22, 0xc000, 20);
        Camera_SetSpeed(0x40000, 0x8000);
        Camera_MoveTo(0x700000, -1, 0x14b0000, 1);
        Engine_CameraWaitForMove();
        for (i = 0; i < 40; i++) {
            SceneActor_SetModeByFrameBit1(Actor_Get(32));
            SceneActor_SetModeByFrameBit1(Actor_Get(33));
            SceneActor_SetModeByFrameBit1(Actor_Get(30));
            SceneActor_SetModeByFrameBit1(Actor_Get(29));
            Engine_TaskWait(1);
        }
        phase = &HaidiaArashi_ShakeShift;
        steps = (s32)FieldScene_RunFourPairedSteps;
        HaidiaArashi_ShakeDone = 0;
        *phase = 0;
        Value2(Scheduler_AddOrUpdateCallback, steps, 0xc80);
        callback = (s32)SceneState_SetValue19ThenCall;
        Scheduler_AddOrUpdateCallback(callback, 0xc80);
        Engine_EventWait(40);
        *phase = 1;
        Engine_EventWait(30);
        Actor_SetPosition(19, 0x720000, 0x1220000);
        record = Actor_Get(19);
        shifted = *(s32 *)(record + 12) + 0x400000;
        *(s32 *)(record + 12) = shifted;
        *(s32 *)(record + 60) = shifted;
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Audio_PlayCue(145);
        Actor_MoveToAndWait(19, 114, 0x14d);
        Engine_ActorSetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Audio_PlayCue(145);
        *phase = 0;
        Actor_SetSpeed(19, 0x6666, 0x3333);
        Actor_MoveToAndWait(19, 114, 0x12c);
        Engine_ActorSetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Audio_PlayCue(145);
        *phase = 2;
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Actor_MoveToAndWait(19, 114, 0x14d);
        Engine_ActorSetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Audio_PlayCue(145);
        *phase = 0;
        Actor_SetSpeed(19, 0x6666, 0x3333);
        Actor_MoveToAndWait(19, 114, 0x12c);
        Engine_ActorSetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Audio_PlayCue(145);
        *phase = 2;
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Actor_MoveToAndWait(19, 114, 0x14d);
        Engine_ActorSetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Audio_PlayCue(145);
        *phase = 1;
        Engine_EventWait(20);
        Actor_SetAttachedEffect(32, 0x102);
        Engine_ActorRunRepeatedMotion(32, 2);
        Event_ShowMessage(31, 0);
        Actor_ShowEmote(33, 0x100, 0);
        Engine_ActorRunRepeatedMotion(33, 2);
        Event_ShowMessageAndWait(28, 0, 40);
        Actor_SetAttachedEffect(30, 0x102);
        Engine_ActorRunRepeatedMotion(30, 2);
        Event_ShowMessage(30, 0);
        HaidiaArashi_ShakeDone = 1;
        Engine_ActorSetAnimation(29, 1);
        Engine_TaskWait(1);
        Actor_SetChildValue(29, 0);
        Actor_ShowEmote(29, 0x105, 20);
        Actor_FaceDirection(29, 0x8000, 40);
        Actor_FaceDirection(29, 0, 20);
        Actor_FaceDirection(29, 0x8000, 20);
        Actor_FaceDirection(29, 0x4000, 40);
        Actor_ShowEmote(29, 0x100, 0);
        Engine_ActorRunRepeatedMotion(29, 2);
        Engine_ActorJump(29, 4, 40);
        Engine_ActorSetAnimation(29, 9);
        Engine_EventWait(10);
        Event_ShowMessageAndWait(29, 0, 20);
        Audio_PlayCue(0x121);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Camera_SetSpeed(0x60000, 0xc000);
        Camera_MoveTo(0x540000, -1, 0x2340000, 1);
        Engine_CameraWaitForMove();
        BattleFx_PlayQueuedSound();
        Actor_FaceActor(22, ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(20);
        Actor_SetAttachedEffect(22, 0x102);
        Engine_EventWait(30);
        Scheduler_RemoveCallback(steps);
        Scheduler_RemoveCallback(callback);
        Event_ShowMessage(22, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
        Engine_EventWait(20);
        FieldScene_RunSingleStep();
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimationAndWait(22, 3);
        Engine_EventWait(20);
        Engine_ActorSetAnimation(22, 2);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(22);
        Actor_SetPosition(22, 0, 0);
        Engine_ActorDestroy(31);
        Engine_ActorDestroy(28);
        Engine_ActorDestroy(30);
        Engine_ActorDestroy(29);
        Engine_ActorDestroy(32);
        Engine_ActorDestroy(33);
        GameFlag_Set(FLAG_BOULDER_FELL);
        Engine_EventEnd();
    }
}

void OverlayObject_SetChildByte5AndMark(u8 *o, s32 n)
{
    if ((*(u8 *)(o + 0x54) & 15) == 1) {
        u8 *c = *(u8 **)(o + 0x50);
        s32 idx = n - 1;
        u8 cnt;
        if (n == 0) {
            idx = HaidiaArashi_FrameModes[(gFrameCount >> 1) & (*(u8 *)(o + 0x54) & 15)];
        }
        cnt = *(u8 *)(c + 0x27);
        if (cnt != 0) {
            u8 **p = (u8 **)(c + 0x28);
            s32 k = cnt;
            do {
                u8 *e = *p++;
                if (e != 0 && *(s32 *)(e + 16) != 0) {
                    *(u8 *)(e + 5) = idx;
                }
                k--;
            } while (k != 0);
        }
        *(u8 *)(c + 0x25) = 1;
    }
}

void ActorPresentation_SetFourActorsModeByBit(void)
{
    if (((gFrameCount >> HaidiaArashi_ShakeShift) & 3) != 0) {
        OverlayObject_SetChildByte5AndMark(Actor_Get(32), 1);
        OverlayObject_SetChildByte5AndMark(Actor_Get(33), 1);
        OverlayObject_SetChildByte5AndMark(Actor_Get(30), 1);
        OverlayObject_SetChildByte5AndMark(Actor_Get(29), 1);
    } else {
        OverlayObject_SetChildByte5AndMark(Actor_Get(32), 8);
        OverlayObject_SetChildByte5AndMark(Actor_Get(33), 8);
        OverlayObject_SetChildByte5AndMark(Actor_Get(30), 8);
        OverlayObject_SetChildByte5AndMark(Actor_Get(29), 8);
    }
}

/* The storm night at the river, until flag 0x83a is set: the villagers are
 * placed along the bank, actor 26 cries out for her brother, the leader and
 * actor 22 come down to look, actor 23 is swept away, and the river search
 * follows. */
void FieldScene_RunFlagGatedActorSequence(void)
{
    u8 *tbl;

    if (Value1(Engine_GameFlagIsSet, 0x83a) != 0) {
        return;
    }
    Engine_EventBegin();
    Call3(Engine_ActorSetPosition, 10, 0xC00000, 0x4BE0000);
    Call3(Engine_ActorFaceDirection, 10, 0x2000, 0);
    Engine_ActorSetAnimation(10, 5);
    {
        u8 *o;
        s32 v;
        o = (u8 *)Object_GetById(10);
        v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
        tbl = HaidiaArashi_ActorEightScript;
        *(u16 *)(o + 0x64) = v;
        Engine_ActorEnableActionCallback(10, (s32)tbl);
    }
    Call3(Engine_ActorSetPosition, 9, 0xC00000, 0x4A50000);
    Call3(Engine_ActorFaceDirection, 9, 0x2000, 0);
    Call3(Engine_ActorSetPosition, 24, 0xE30000, 0x4BE0000);
    Call3(Engine_ActorFaceDirection, 24, 0x4000, 0);
    Engine_ActorSetAnimation(24, 6);
    {
        u8 *o;
        s32 v;
        o = (u8 *)Object_GetById(24);
        v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Engine_ActorEnableActionCallback(24, (s32)tbl);
    }
    Call3(Engine_ActorSetPosition, 25, 0xFA0000, 0x4BE0000);
    Call3(Engine_ActorFaceDirection, 25, 0x4000, 0);
    Engine_ActorSetAnimation(25, 6);
    {
        u8 *o;
        s32 v;
        o = (u8 *)Object_GetById(25);
        v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Engine_ActorEnableActionCallback(25, (s32)tbl);
    }
    Call3(Engine_ActorSetPosition, 26, 0xE30000, 0x4A50000);
    Call3(Engine_ActorFaceDirection, 26, 0x3000, 0);
    Call3(Engine_ActorSetPosition, 23, 0xF30000, 0x4FD0000);
    Call3(Engine_ActorFaceDirection, 23, 0xC000, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(23), 0);
    Engine_TaskWait(3);
    Engine_EventSetMessage((s32)MsgHaidiaNoBrother);
    Engine_EventShowMessage(0x201a, 0);
    Call3(Engine_ActorShowEmote, ACTOR_PARTY_LEADER, 0x100, 20);
    Call3(Engine_ActorWalkToAndWait, ACTOR_PARTY_LEADER, 150, 0x446);
    {
        u8 *p;
        p = (u8 *)Object_GetById(ACTOR_PARTY_LEADER);
        if (p != 0) {
            Engine_ActorSetPosition(22, *(s32 *)(p + 8), *(s32 *)(p + 16));
        }
    }
    Call3(Engine_ActorWalkToAndWait, 22, 132, 0x446);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, ACTOR_PARTY_LEADER, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 22, 0x4000, 20);
    Call2(Engine_CameraSetSpeed, 0x40000, 0x8000);
    Call4(Engine_CameraMoveTo, 0xD80000, -1, 0x4D00000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventShowMessageAndWait(10, 0, 10);
    Engine_ActorRunRepeatedMotion(23, 3);
    Engine_ActorFaceDirection(9, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Call3(Engine_ActorFaceDirection, 9, 0x3000, 10);
    Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
    Call4(Engine_CameraMoveTo, 0xE80000, -1, 0x4E50000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_AudioPlayCue(134);
    Engine_ActorJump(23, 4, 0);
    Engine_ActorSetAnimation(23, 6);
    Engine_EventWait(10);
    Engine_ActorSetPosition(23, 0, 0);
    Engine_EventWait(60);
    BattleFx_PlayQueuedSound();
    Engine_ActorSetAnimation(10, 1);
    {
        u8 *o;
        o = (u8 *)Object_GetById(10);
        *(s32 *)(o + 0x18) = 0x10000;
        *(s32 *)(o + 0x1C) = 0x10000;
    }
    Engine_ActorSetAnimation(24, 1);
    {
        u8 *o;
        o = (u8 *)Object_GetById(24);
        *(s32 *)(o + 0x18) = 0x10000;
        *(s32 *)(o + 0x1C) = 0x10000;
    }
    Engine_ActorSetAnimation(25, 1);
    {
        u8 *o;
        o = (u8 *)Object_GetById(25);
        *(s32 *)(o + 0x18) = 0x10000;
        *(s32 *)(o + 0x1C) = 0x10000;
    }
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_ActorStartRepeatedMotion(9, 2);
    Engine_ActorStartRepeatedMotion(24, 2);
    Engine_ActorStartRepeatedMotion(25, 2);
    Engine_ActorRunRepeatedMotion(26, 2);
    Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
    Call4(Engine_CameraMoveTo, 0xD80000, -1, 0x4D00000, 1);
    Engine_CameraWaitForMove();
    Call2(Engine_ActorSetAttachedEffect, 26, 0x102);
    Engine_ActorSetAttachedEffect(9, 0x102);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(26, 2);
    Engine_ActorStartRepeatedMotion(26, 3);
    Engine_EventShowMessage(26, 0);
    Engine_ActorJump(25, 2, 0);
    Call3(Engine_ActorSetDestination, 25, 234, 0x4B5);
    Engine_ActorJump(26, 2, 0);
    Call3(Engine_ActorSetDestination, 26, 227, 0x4B1);
    Engine_EventWait(90);
    Call4(Engine_CameraMoveTo, 0xE80000, -1, 0x4E50000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetPosition(23, 0xF30000, 0x4FD0000);
    Engine_TaskWait(1);
    Engine_AudioPlayCue(106);
    {
        u8 *o;
        o = (u8 *)Object_GetById(23);
        *(s32 *)(o + 0x28) = 0x20000;
    }
    Engine_EventWait(6);
    Engine_ActorSetAnimation(23, 7);
    Engine_EventWait(20);
    BattleFx_PlayQueuedSound();
    Engine_EventWait(20);
    Engine_CameraSetSpeed(0x19999, 0x3333);
    Engine_CameraMoveTo(0xD80000, -1, 0x4D00000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorRunRepeatedMotion(24, 2);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 24, 0x105, 40);
    Engine_ActorFaceEachOther(24, 10, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Call2(Engine_EventShowMessage, 0x800A, 0);
    {
        u8 *o;
        o = (u8 *)Object_GetById(25);
        o[0x5A] &= 0xFE;
    }
    {
        u8 *o;
        o = (u8 *)Object_GetById(26);
        o[0x5A] &= 0xFE;
    }
    Call3(Engine_ActorSetSpeed, 25, 0x9999, 0x4CCC);
    Call3(Engine_ActorSetSpeed, 26, 0x9999, 0x4CCC);
    Call3(Engine_ActorSetDestination, 25, 247, 0x4BA);
    Call3(Engine_ActorMoveToAndWait, 26, 227, 0x4A5);
    {
        u8 *o;
        o = (u8 *)Object_GetById(25);
        o[0x5A] |= 1;
    }
    {
        u8 *o;
        o = (u8 *)Object_GetById(26);
        o += 0x5A;
        /* FAKEMATCH: the set bit first, so the or writes into the register
           that holds it, as the ROM's does. */
        {
            u8 set = 1 | *o;
            *o = set;
        }
    }
    Engine_ActorFaceDirection(26, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x8000, 10);
    Engine_ActorSetAnimation(24, 4);
    Engine_EventShowMessageAndWait(0x8018, 0, 10);
    Call3(Engine_ActorFaceDirection, 10, 0xC000, 20);
    Engine_ActorFaceDirection(10, 0, 10);
    Engine_ActorSetAnimation(10, 4);
    Call3(Engine_EventShowMessageAndWait, 0x800A, 0, 10);
    Call3(Engine_ActorShowEmote, 24, 0x105, 0);
    Call3(Engine_ActorShowEmote, 10, 0x105, 60);
    Call3(Engine_ActorShowEmote, 9, 0x106, 20);
    Call3(Engine_ActorFaceDirection, 9, 0x8000, 40);
    Call3(Engine_ActorFaceDirection, 9, 0xC000, 20);
    Engine_ActorFaceDirection(9, 0, 30);
    Call3(Engine_ActorFaceDirection, 9, 0x4000, 10);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Call3(Engine_ActorFaceDirection, 10, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x9000, 0);
    Call3(Engine_ActorFaceDirection, 24, 0xA000, 0);
    Call3(Engine_ActorFaceDirection, 26, 0x8000, 10);
    Engine_ActorRunRepeatedMotion(10, 1);
    Call3(Engine_EventShowMessageAndWait, 0x800A, 0, 10);
    Engine_ActorSetAnimation(9, 4);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Call3(Engine_ActorShowEmote, 10, 0x105, 0);
    Call3(Engine_ActorShowEmote, 24, 0x105, 0);
    Call3(Engine_ActorShowEmote, 25, 0x105, 0);
    Call3(Engine_ActorShowEmote, 26, 0x105, 40);
    Engine_ActorFaceDirection(9, 0, 10);
    Engine_ActorFaceEachOther(24, 25, 0);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(9, 0, 0);
    Engine_ActorFaceDirection(10, 0, 10);
    Call3(Engine_ActorFaceDirection, 24, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x8000, 10);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimationAndWait(25, 3);
    Engine_ActorFaceEachOther(10, 9, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventShowMessageAndWait(0x800A, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 3);
    Call3(Engine_ActorFaceDirection, 24, 0xD000, 10);
    Engine_ActorStartRepeatedMotion(24, 1);
    Engine_EventShowMessageAndWait(24, 0, 10);
    Engine_ActorFaceDirection(10, 0, 0);
    Engine_ActorFaceDirection(9, 0, 0);
    Engine_ActorRunRepeatedMotion(26, 1);
    Call3(Engine_ActorFaceDirection, 26, 0x2000, 20);
    Call3(Engine_ActorFaceDirection, 25, 0xA000, 20);
    Engine_ActorSetAnimationAndWait(25, 3);
    Engine_EventShowMessageAndWait(25, 0, 10);
    Engine_ActorSetAnimationAndWait(26, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Call3(Engine_ActorFaceDirection, 26, 0x8000, 10);
    Engine_ActorSetAnimationAndWait(26, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventShowMessageAndWait(9, 0, 10);
    HaidiaArashi_RunRiverSearch();
    Engine_GameFlagSet(0x83A);
    Engine_EventEnd();
}

/* The storm night by the river: while the cells shake, actors 9 and 26
 * run their scripts, actors 10, 24 and 25 each get a random count of 60
 * to 149 and actor 8's script, actor 9 says she will look north and
 * sends the others to the plaza, and asks the party twice for help before
 * actor 22 follows the leader out. */
void HaidiaArashi_RunRiverSearch(void)
{
    s32 entry;
    s32 record;
    s32 script;
    s32 north;
    s32 suppose;

    Actor_FaceDirection(26, 0x3000, 0);
    Actor_FaceDirection(24, 0xd000, 0);
    Actor_FaceDirection(25, 0xb000, 0);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_FaceDirection(10, 0xd000, 20);
    Engine_ActorSetAnimation(26, 3);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimation(25, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(25, 3);
    Engine_EventWait(20);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x860000, -1, 0x4ab0000, 1);
    Actor_SetSpeed(26, 0x19999, 0xcccc);
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    Engine_ActorEnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptA);
    Object_SetActionCallbackAndRefreshById(9, (s32)HaidiaArashi_ActorNineScriptA);
    Audio_PlayCue(158);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps2, 38, 72);
    Engine_EventWait(10);
    Actor_WalkToAndWait(9, 149, 0x497);
    Actor_SetPosition(9, 0, 0);
    Actor_WalkToAndWait(25, 250, 0x4be);
    BattleFx_PlayQueuedSound();
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(24, 0x3000, 0);
    Actor_FaceDirection(25, 0x3000, 0);
    Engine_ActorSetAnimation(10, 5);
    Engine_ActorSetAnimation(24, 6);
    Engine_ActorSetAnimation(25, 6);
    /* For records 10, 24 and 25: fetch the record's entry pointer, fetch a
     * value from that record's own state, and store a derived value into
     * the entry's field at offset 100. */
    entry = (s32)Actor_Get(10);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (Math_RemainderUnsigned(record, 90) + 60);
    entry = (s32)Actor_Get(24);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (Math_RemainderUnsigned(record, 90) + 60);
    entry = (s32)Actor_Get(25);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (Math_RemainderUnsigned(record, 90) + 60);
    script = (s32)HaidiaArashi_ActorEightScript;
    Engine_ActorEnableActionCallback(10, (const u8 *)script);
    Engine_ActorEnableActionCallback(24, (const u8 *)script);
    Engine_ActorEnableActionCallback(25, (const u8 *)script);
    Object_RefreshSelectorById(26);
    Engine_EventWait(10);
    Audio_PlayCue(159);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps5, 38, 72);
    Engine_EventWait(30);
    BattleFx_PlayQueuedSound();
    Camera_MoveTo(0x700000, -1, 0x4c90000, 1);
    Audio_PlayCue(158);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps1, 35, 73);
    Engine_EventWait(20);
    BattleFx_PlayQueuedSound();
    Engine_ActorEnableActionCallback(9, HaidiaArashi_ActorNineScriptB);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptB);
    Engine_EventWait(40);
    Audio_PlayCue(159);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps4, 35, 73);
    Object_RefreshSelectorById(26);
    BattleFx_PlayQueuedSound();
    Engine_EventWait(40);
    north = (s32)MsgHaidiaGoLookNorth;
    Engine_EventSetMessage(north);
    Event_ShowMessageAndWait(9, 0, 20);
    Engine_ActorSetAnimationAndWait(26, 3);
    Event_ShowMessageAndWait(0x201a, 0, 40);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(26, 3);
    Engine_EventWait(30);
    Engine_ActorEnableActionCallback(9, HaidiaArashi_ActorNineScriptC);
    Engine_ActorEnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptC);
    Engine_EventWait(40);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x690000, -1, 0x43e0000, 1);
    Object_RefreshSelectorById(9);
    Actor_FaceDirection(9, 0, 0);
    Actor_ShowEmote(9, 0x100, 40);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(22, 0x8000, 10);
    Actor_WalkToAndWait(9, 105, 0x43e);
    Engine_ActorRunRepeatedMotion(9, 2);
    Event_OpenMessage(0x8009, 0);
    Actor_FaceDirection(22, 0, 0);
    /* Branch on a condition; pass byte 4 or byte 5 of the 0xe9b
     * table to the corresponding follow-up call. */
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(9, 3);
        Engine_EventSetMessage((north + 4));
    } else {
        Engine_ActorRunRepeatedMotion(9, 2);
        Engine_EventSetMessage((north + 5));
    }
    Event_ShowMessage(0x8009, 0);
    Actor_FaceDirection(22, 0x8000, 40);
    Actor_ShowEmote(9, 0x100, 30);
    suppose = (s32)MsgHaidiaDontSupposeTwo;
    Engine_EventSetMessage(suppose);
    Event_OpenMessage(0x8009, 0);
    /* Branch on a condition; each side reads a different byte of the
     * 0xea1 table and runs its own follow-up sequence. */
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(9, 3);
        Engine_EventSetMessage((suppose + 1));
        Event_ShowMessageAndWait(0x8009, 0, 30);
        Actor_FaceDirection(22, 0x8000, 20);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimation(22, 3);
        Engine_ActorSetAnimationAndWait(9, 3);
        Engine_EventWait(40);
    } else {
        Actor_ShowEmote(9, 0x105, 90);
        Actor_ShowEmote(9, 0x103, 40);
        Engine_ActorSetAnimation(9, 4);
        Engine_EventSetMessage((suppose + 2));
        Event_ShowMessage(0x8009, 0);
    }
    Engine_ActorEnableActionCallback(9, HaidiaArashi_ActorNineScriptD);
    Engine_EventWait(90);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(22, 2);
    /* If a record pointer is returned, pass its s16 fields at offsets 10
     * and 18 through to the follow-up call. */
    record = (s32)Actor_Get(0);
    if (record != 0) {
        Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(22);
    Actor_SetPosition(22, 0, 0);
}

void SceneDialogue_RunActorTenFlag30dDialogue(void)
{
    s32 v2000 = 0x2000;
    u8 *tbl;

    Engine_EventBegin();
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 20);
    if (GameFlag_IsSet(0x30d) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaTwoDontEnough);
        Event_ShowMessageAndWait(10, 0, 10);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaOh);
        Engine_ActorStartRepeatedMotion(10, 1);
        Event_ShowMessageAndWait(10, 0, 10);
        Engine_ActorStartRepeatedMotion(10, 2);
        Event_ShowMessageAndWait(10, 0, 10);
    }
    Actor_FaceDirection(10, v2000, 20);
    Engine_ActorSetAnimation(10, 5);
    Engine_EventWait(10);
    {
        u8 *rec;
        s32 v;
        rec = Actor_Get(10);
        v = Math_RemainderUnsigned(Random_Next(), 0x5A) + 60;
        tbl = HaidiaArashi_ActorEightScript;
        *(u16 *)(rec + 0x64) = v;
        Engine_ActorEnableActionCallback(10, tbl);
    }
    Engine_EventWait(20);
    GameFlag_Set(0x30d);
    Engine_EventEnd();
}

void HaidiaArashi_RunScene02DEC(void)
{
    u32 i;
    u8 *rec;
    u8 *record;
    s32 v5;
    u8 *p4;

    if (Engine_GameFlagIsSet(0x840) == 0) {
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x841) != 0) {
        } else {
            Engine_EventBegin();
            Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 22, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 26, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 8, 0x10000, 0x8000);
            Call3(Engine_ActorWalkToAndWait, 0, 217, 0x557);
            record = (s32)Object_GetById(0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(22, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 22, 235, 0x557);
            Call3(Engine_ActorFaceDirection, 22, 0xb000, 0);
            record = (s32)Object_GetById(0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(26, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 26, 199, 0x557);
            Call3(Engine_ActorFaceDirection, 26, 0xd000, 0);
            Call3(Engine_ActorSetPosition, 25, 0xf70000, 0x4ba0000);
            Engine_ActorFaceDirection(25, 0x6000, 0);
            record = (s32)Object_GetById(8);
            p4 = *(s32 *)((s32)record + 80);
            ((struct Flags35 *)record)->flags &= 254;
            ((struct Flags9 *)p4)->mode = 1;
            rec = (s32)Object_GetById(0);
            p4 = *(s32 *)((s32)rec + 80);
            ((struct Flags35 *)rec)->flags &= 254;
            ((struct Flags9 *)p4)->mode = 2;
            record = (s32)Object_GetById(0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(8, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 8, 221, 0x569);
            Call3(Engine_ActorFaceDirection, 8, 0xb000, 60);
            Engine_ActorRunRepeatedMotion(26, 2);
            Engine_EventSetMessage((s32)MsgHaidiaMomDadBack);
            Engine_EventShowMessageAndWait(26, 0, 40);
            Call3(Engine_ActorSetPosition, 9, 0x650000, 0x4ad0000);
            Call3(Engine_ActorFaceDirection, 9, 0x2000, 0);
            Engine_EventShowMessageAndWait(0x1009, 0, 10);
            Call3(Engine_ActorFaceDirection, 26, 0xa000, 0);
            Engine_CameraSetSpeed(0x13333, 0x2666);
            Call4(Engine_CameraMoveTo, 0x650000, -1, 0x4ad0000, 1);
            Call3(Engine_ActorSetSpeed, 9, 0x16666, 0xb333);
            Engine_ActorEnableActionCallback(9, (s32)HaidiaArashi_StormRunActions);
            Engine_EventWait(60);
            Engine_CameraSetSpeed(0x9999, 0x1333);
            Call4(Engine_CameraMoveTo, 0xbb0000, -1, 0x5300000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(40);
            Engine_ActorRunRepeatedMotion(26, 2);
            Engine_EventShowMessageAndWait(26, 0, 20);
            Engine_ActorRunRepeatedMotion(9, 2);
            Engine_EventShowMessageAndWait(0x4009, 0, 20);
            Engine_CameraSetSpeed(0x20000, 0x4000);
            Call4(Engine_CameraMoveTo, 0xdd0000, -1, 0x5690000, 1);
            Engine_ActorFaceActor(0, 8, 0);
            Engine_ActorFaceActor(22, 8, 0);
            Call3(Engine_ActorFaceDirection, 26, 0x3000, 80);
            Engine_CameraMoveTo(0xb60000, -1, 0x5500000, 1);
            Call3(Engine_ActorWalkToAndWait, 8, 182, 0x568);
            Engine_ActorFaceActor(8, 9, 0);
            Engine_EventWait(30);
            Engine_ActorSetAnimationAndWait(8, 3);
            Engine_EventWait(10);
            Engine_ActorFaceActor(0, 9, 0);
            Engine_ActorFaceActor(22, 9, 0);
            Engine_ActorFaceActor(26, 9, 0);
            Engine_ActorSetAnimationAndWait(9, 3);
            Engine_EventShowMessage(9, 0);
            Engine_ActorRunRepeatedMotion(26, 2);
            Engine_EventShowMessageAndWait(26, 0, 10);
            Call3(Engine_ActorFaceDirection, 9, 0xe000, 40);
            Call3(Engine_ActorFaceDirection, 9, 0x3000, 20);
            Engine_ActorSetAnimationAndWait(9, 3);
            Engine_EventShowMessage(9, 0);
            Engine_ActorFaceEachOther(26, 8, 0);
            Engine_ActorFaceEachOther(22, 0, 0);
            Engine_EventWait(40);
            Engine_ActorFaceActor(0, 9, 0);
            Engine_ActorFaceActor(22, 9, 0);
            Engine_ActorFaceActor(26, 9, 0);
            Engine_ActorFaceActor(8, 9, 0);
            Engine_ActorRunRepeatedMotion(9, 2);
            Engine_EventWait(20);
            Engine_EventShowMessageAndWait(9, 0, 10);
            Engine_ActorSetAnimation(0, 3);
            Engine_ActorSetAnimation(26, 3);
            Engine_ActorSetAnimation(22, 3);
            v5 = 1;
            Engine_ActorSetAnimationAndWait(8, 3);
            rec[35] |= v5;
            record = (s32)Object_GetById(8);
            record[35] |= v5;
            Scene_RunActorGroupDepartureSequence();
            Engine_GameFlagSet(0x841);
            Engine_EventEnd();
        }
    }
}
