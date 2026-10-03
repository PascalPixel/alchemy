#include "GROUP_DEPARTURE.H"
#include "TYPES.H"
#include "CALL.H"
#include "FIELD_SERVICE.H"

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
        object->x = Engine_RandomNext();
        counter = Math_RemainderUnsigned(Engine_RandomNext(), 20) + 20;
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
        state->countdown = (s16)(Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60);
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

    Engine_GameFlagSet(0x210);
    a = 10;
    b = 84;
    Engine_MapCopyCellAttributes(40, 84, 7, 4, a, b);
}

void SceneState_SetFlag210AndConfigureRegion40_89(void)
{

    s32 a;
    s32 b;

    Engine_GameFlagClear(0x210);
    a = 10;
    b = 84;
    Engine_MapCopyCellAttributes(40, 89, 7, 4, a, b);
}

void SceneState_SetWork1c0AndRunObject(u8 *o)
{

    u8 *state;

    if (Engine_GameFlagIsSet(0x834) != 0) {
        BattleFx_SetBlock30ValuesMaxZero();
    }
    state = (u8 *)gEventWork;
    *(s32 *)(state + 0x1C0) = 0x100;
    *(s32 *)(state + 0x1C8) = 16;
    Engine_EventRequestExit(o);
}

void FieldScene_SetupDescriptorD774(void)
{
    Engine_AudioPlayCue(0x9E);
    Engine_MapAnimateCells(HaidiaArashi_CellSteps0, 45, 11);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x101, 0x1A4);
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(11);
}

void SceneState_SetValue123Mode1(void)
{

    Engine_AudioPlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(1);
}

void SceneState_ApplyValues123And3(void)
{

    Engine_AudioPlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(3);
}

void SceneState_SetValue123Mode4(void)
{

    Engine_AudioPlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(4);
}

void FieldScene_RunStep7BAndCheckFlags841And842(void)
{
    Engine_AudioPlayCue(0x7B);
    if (Engine_GameFlagIsSet(0x841) != 0
        && Engine_GameFlagIsSet(0x842) == 0) {
        FieldScene_ConfigureActorTwentyTwoScene();
    }
    SceneState_SetWork1c0AndRunObject(2);
}

void FieldScene_SetupDescriptorD78a(void)
{
    Engine_AudioPlayCue(0x9E);
    Engine_MapAnimateCells(HaidiaArashi_CellSteps1, 54, 32);
    Engine_ActorWalkTo(0, 0x196, 0x2d7);
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(5);
}

void FieldScene_RunScene372_02000278(void)
{
    u32 i;
    s32 record;

    if (Engine_GameFlagIsSet(0x206) == 0) {
        Engine_AudioPlayCue(158);
        Engine_MapAnimateCells(HaidiaArashi_CellSteps2, 45, 39);
    }
    if (Engine_GameFlagIsSet(0x835) == 0) {
        record = Engine_GameFlagIsSet(0x831);
        if (record != 0) {
            goto L_020002b4;
        }
        FieldScene_RunScene372SequenceA();
        Engine_GameFlagSet(0x206);
    } else {
        L_020002b4:;
        Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x106, 0x325);
        Engine_EventWait(3);
        SceneState_SetWork1c0AndRunObject(6);
    }
}

void FieldScene_SetupDescriptorD78aIfFlag205Clear(void)
{
    if (Engine_GameFlagIsSet(0x205) == 0) {
        Engine_AudioPlayCue(0x9E);
        Engine_MapAnimateCells(HaidiaArashi_CellSteps1, 50, 44);
    }
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x154, 0x378);
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(7);
}

void FieldScene_SetupWithDescriptorD7A0(void)
{
    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(HaidiaArashi_CellSteps2, 49, 69);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x146, 0x466);
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(8);
}

void FieldScene_SetupDescriptorD7b6(void)
{
    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(HaidiaArashi_CellSteps3, 52, 76);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x176, 0x4d6);
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(9);
}

void FieldScene_RunScene372_02000398(void)
{
    u32 i;
    s32 record;

    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(HaidiaArashi_CellSteps1, 35, 74);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(10);
}

void FieldScene_RunScene372_020003cc(void)
{
    u32 i;
    s32 record;

    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(HaidiaArashi_CellSteps1, 35, 73);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
    Engine_EventWait(3);
    SceneState_SetWork1c0AndRunObject(12);
}

void FieldScene_RunScene372_02000400(void)
{
    u32 i;
    s32 record;

    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(HaidiaArashi_CellSteps2, 38, 72);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 146, 0x49e);
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
    Engine_ActorSetPosition(23, 0, 0);
    if (Engine_GameFlagIsSet(0x109) != 0) {
        Engine_GameFlagClear(0x205);
        Engine_GameFlagClear(0x206);
    }
    if (Engine_GameFlagIsSet(0x830) != 0) {
        Engine_ActorSetPosition(11, w1, w2);
        HaidiaArashi_SetStormCellAttributes();
    }
    if (Engine_GameFlagIsSet(0x831) != 0) {
        Engine_ActorSetPosition(12, w3, w4);
        ActorPresentation_SetEightSceneCells();
    }
    if (Engine_GameFlagIsSet(0x832) != 0) {
        Engine_ActorSetPosition(13, w5, p1);
        SceneState_ApplyFourRects();
    }
    if (Engine_GameFlagIsSet(0x833) != 0) {
        Engine_ActorSetPosition(14, w6, p2);
        FieldScene_DrawFiveTileBlocks();
    }
    {
        u8 *q;
        q = Object_GetById(11);
        q += 0x59;
        m = 4;
        *q = *q | m;
        q = Object_GetById(12);
        q += 0x59;
        *q = *q | m;
        q = Object_GetById(13);
        q += 0x59;
        *q = *q | m;
        q = Object_GetById(14);
        q += 0x59;
        *q = *q | m;
        q = Object_GetById(15);
        q += 0x59;
        *q = *q | m;
        q = Object_GetById(16);
        q += 0x59;
        *q = *q | m;
        q = Object_GetById(17);
        q += 0x59;
        *q = *q | m;
        q = Object_GetById(18);
        q += 0x59;
        { u8 tmp = m | *q; *q = tmp; }
    }
    if (Engine_GameFlagIsSet(0x837) != 0) {
        Engine_ActorSetPosition(22, 0, 0);
    }
    {
        u8 *q;
        q = Object_GetById(19);
        *(s32 *)(q + 0x18) = 0x20000;
        *(s32 *)(q + 0x1C) = 0x20000;
    }
    if (Engine_GameFlagIsSet(FLAG_BOULDER_FELL) != 0) {
        Engine_ActorSetPosition(19, w7, p3);
    } else {
        Engine_ActorEnableActionCallback(19, HaidiaArashi_ActorNineteenScript);
    }
    if (Engine_GameFlagIsSet(0x841) != 0) {
        s32 h2;
        u8 *tbl;
        FieldScene_BuildPlacementGrid();
        Engine_ActorSetPosition(9, w21, 0x4CD0000);
        {
            u8 *o;
            s32 v;
            o = Object_GetById(9);
            h2 = 0xE000;
            *(u16 *)(o + 6) = h2;
            v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            tbl = HaidiaArashi_ActorEightScript;
            o += 0x66;
            t = 1;
            *(u16 *)o = t;
            Engine_ActorEnableActionCallback(9, tbl);
        }
        Engine_ActorSetPosition(26, w22, 0x4E60000);
        {
            u8 *o;
            s32 v;
            o = Object_GetById(26);
            *(u16 *)(o + 6) = h2;
            v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 2;
            *(u16 *)o = t;
            Engine_ActorEnableActionCallback(26, tbl);
        }
        Engine_ActorSetPosition(22, 0x980000, 0x5050000);
        {
            u8 *o;
            s32 v;
            o = Object_GetById(22);
            *(u16 *)(o + 6) = h2;
            v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 3;
            *(u16 *)o = t;
            Engine_ActorEnableActionCallback(22, tbl);
        }
        Engine_ActorSetPosition(8, 0xB80000, 0x5180000);
        {
            u8 *o;
            s32 v;
            o = Object_GetById(8);
            *(u16 *)(o + 6) = h2;
            v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 4;
            *(u16 *)o = t;
            Engine_ActorEnableActionCallback(8, tbl);
        }
        Engine_ActorSetAnimation(8, 6);
        {
            u8 *r;
            r = Object_GetById(22);
            r += 0x23;
            m2 = 0xFE;
            *r = *r & m2;
            r = Object_GetById(8);
            r += 0x23;
            *r = m2 & *r;
        }
        Scheduler_AddOrUpdateCallback(OverlayObject_CopyRecordField1ToSlots22And8, 0xC80);
        Engine_ActorSetPosition(24, 0, 0);
        Engine_ActorSetPosition(25, 0, 0);
        Engine_ActorSetPosition(23, 0, 0);
        Engine_ActorSetPosition(19, 0, 0);
        if (Engine_GameFlagIsSet(0x842) != 0) {
            Engine_ActorSetPosition(22, 0, 0);
        }
    } else if (Engine_GameFlagIsSet(0x83a) != 0) {
        u8 *tbl;
        Engine_ActorSetPosition(10, w8, b1);
        Engine_ActorFaceDirection(10, w9, 0);
        Engine_ActorSetAnimation(10, 5);
        {
            u8 *o;
            s32 v;
            o = Object_GetById(10);
            v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        tbl = HaidiaArashi_ActorEightScript;
        Engine_ActorEnableActionCallback(10, tbl);
        Engine_ActorSetPosition(24, w10, b2);
        Engine_ActorFaceDirection(24, w11, 0);
        Engine_ActorSetAnimation(24, 6);
        {
            u8 *o;
            s32 v;
            o = Object_GetById(24);
            v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Engine_ActorEnableActionCallback(24, tbl);
        Engine_ActorSetPosition(25, w12, b3);
        Engine_ActorFaceDirection(25, w13, 0);
        Engine_ActorSetAnimation(25, 6);
        {
            u8 *o;
            s32 v;
            o = Object_GetById(25);
            v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Engine_ActorEnableActionCallback(25, tbl);
        Engine_ActorSetPosition(23, w14, p4);
        Engine_ActorFaceDirection(23, 0xC000, 0);
        Engine_ActorSetSpriteFlags(Object_GetById(23), 0);
        Engine_ActorSetPosition(17, 0, 0);
        Engine_ActorSetPosition(18, 0, 0);
    } else {
        Engine_ActorSetPosition(17, 0, 0);
        Engine_ActorSetPosition(18, 0, 0);
    }
    {
        s16 *table = (s16 *)&gGameState;
        if (table[225] != 15 || Engine_GameFlagIsSet(0x87b) != 0) {
            Effect_SoundAndFlash();
            BattleFx_StartTwelveFrameBlend();
        }
    }
    if (Engine_GameFlagIsSet(0x210) != 0) {
        SceneState_SetFlag210AndConfigureRegion40_84();
    }
    Engine_GameFlagSet(0x834);
    k = 46;
    Engine_MapCopyCellAttributes(29, 24, 1, 2, 26, k);
    Engine_MapCopyCellAttributes(29, 25, 1, 1, 27, k);
    Engine_MapCopyCellAttributes(29, 25, 1, 1, 28, k);
    k = 20;
    Engine_MapCopyCellAttributes(19, 0x5A, 1, 1, k, 0x58);
    Engine_MapCopyCellAttributes(19, 0x5A, 1, 1, k, 0x59);
    {
        u8 *o;
        u8 *q;
        o = Object_GetById(21);
        q = o + 0x55;
        *q = 0;
        *(s32 *)(o + 0xC) = 0xC00000;
        q += 4;
        *q = 8;
        Engine_ActorSetSpriteFlags(o, 0);
    }
    Engine_TaskWait(1);
    if (Engine_GameFlagIsSet(0x87b) == 0) {
        s16 *table = (s16 *)&gGameState;
        if (table[225] == 15) {
            Scene_DoraSendsRobinToThePlaza();
            return 0;
        }
    }
    Engine_ActorSetAnimation(23, 7);
    if (Engine_GameFlagIsSet(0x837) == 0) {
        Engine_EventBegin();
        Engine_ActorSetAttachedEffect(22, c2);
        Engine_ActorSetPosition(22, w15, p5);
        Engine_ActorSetPosition(21, w16, p6);
        Engine_ActorWalkTo(22, w17, c1);
        Engine_ActorWalkToAndWait(21, w18, 0x26B);
        Engine_ActorSetAnimation(21, 2);
        Engine_ActorSetAnimation(22, 5);
        Engine_EventEnd();
    } else {
        Engine_EventBegin();
        Engine_ActorSetPosition(21, w19, p7);
        Engine_ActorWalkToAndWait(21, w20, c3);
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
    Engine_CameraSetSpeed(0x4000, 0x800);
    Engine_CameraMoveTo(0x13c0000, 0xa00000, 0x3700000, 1);
    Engine_ActorSetPosition(ACTOR_KYLE, 0x1260000, 0x3640000);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 16;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    BattleFx_SetBlock30Values128One();
    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells((const u16 *)HaidiaArashi_CellSteps1, 50, 44);
    Engine_ActorSetAttachedEffect(22, 0x101);
    Engine_ActorSetSpeed(ACTOR_DORA, 0xcccc, 0x6666);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Engine_ActorSetSpeed(ACTOR_KYLE, 0xcccc, 0x6666);
    Engine_ActorSetPosition(ACTOR_DORA, 0x1560000, 0x37a0000);
    Engine_ActorWalkToAndWait(ACTOR_DORA, 0x156, 0x389);
    BattleFx_PlayQueuedSound();
    Engine_ActorWalkTo(ACTOR_DORA, 0x128, 0x389);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0x1560000, 0x37a0000);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x156, 0x37a);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x156, 0x389);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x13e, 0x389);
    Engine_ActorSetAnimation(ACTOR_DORA, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Engine_ActorFaceDirection(ACTOR_DORA, 0xc000, 60);
    message = (s32)MsgHaidiaKyleAbleStop;
    Engine_EventSetMessage(message);
    Engine_EventShowMessage(ACTOR_DORA, 0);
    Engine_ActorWalkToAndWait(ACTOR_KYLE, 0x126, 0x346);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_KYLE, 4);
    Engine_EventShowMessage(ACTOR_KYLE, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(ACTOR_KYLE, 0x4000, 0);
    Engine_EventShowMessageAndWait(ACTOR_KYLE, 0, 20);
    Engine_ActorShowEmote(ACTOR_DORA, 0x101, 20);
    Engine_ActorFaceDirection(ACTOR_DORA, 0xc000, 10);
    Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_KYLE, 4);
    Engine_EventShowMessage(ACTOR_KYLE, 0);
    Engine_ActorSetAttachedEffect(ACTOR_DORA, 0x102);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(ACTOR_DORA, 0, 50);
    Engine_ActorFaceDirection(ACTOR_DORA, 0xc000, 10);
    Engine_ActorSetSpeed(ACTOR_DORA, 0x18000, 0xc000);
    Engine_ActorWalkToAndWait(ACTOR_DORA, 0x121, 0x373);
    Engine_ActorFaceDirection(ACTOR_DORA, 0xe000, 0);
    Engine_EventShowMessage(ACTOR_DORA, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_KYLE, 2);
    Engine_EventShowMessage(ACTOR_KYLE, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
    Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorFaceDirection(ACTOR_DORA, 0x2000, 10);
    Engine_EventSetMessage((message + 8));
    Engine_EventOpenMessage(ACTOR_DORA, 0);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x12e, 0x389);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    while (Engine_EventChooseYesNo(0, 0) == 1) {
        Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
        Engine_EventSetMessage((s32)MsgHaidiaBigBoyWhy);
        Engine_EventOpenMessage(ACTOR_DORA, 0);
    }
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Engine_EventSetMessage((s32)MsgHaidiaKnowWayGo);
    Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetSpeed(ACTOR_KYLE, 0x18000, 0xc000);
    Engine_ActorWalkTo(ACTOR_KYLE, 0x129, 0x2ee);
    Engine_EventWait(10);
    Engine_ActorWalkToAndWait(ACTOR_DORA, 0x129, 0x2ee);
    Engine_ActorSetPosition(ACTOR_DORA, 0, 0);
    Engine_ActorSetPosition(ACTOR_KYLE, 0, 0);
    Engine_ActorSetAnimation(ACTOR_KYLE, 1);
    Engine_ActorSetAnimation(21, 2);
    Engine_ActorSetAnimation(22, 5);
    Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
    Engine_GameFlagSet(0x87b);
    Engine_GameFlagSet(0x205);
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

    Engine_MapCopyCellAttributes(29, 64, 1, 1, 21, 57);
    Engine_MapCopyCellAttributes(29, 64, 1, 1, 21, 58);
    Engine_MapCopyCellAttributes(29, 64, 1, 1, 22, 58);
    Engine_MapCopyCellAttributes(29, 64, 1, 1, 20, 58);
    Engine_MapCopyCellAttributes(28, 20, 1, 1, 20, 57);
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

    Engine_MapCopyCellAttributes(29, 23, 1, 1, a, d);
    b = 14;
    Engine_MapCopyCellAttributes(29, 23, 1, 1, b, d);
    c = 13;
    Engine_MapCopyCellAttributes(29, 23, 1, 1, c, d);
    Engine_MapCopyCellAttributes(26, 20, 2, 1, b, 0x34);
    e = 0x36;
    Engine_MapCopyCellAttributes(25, 21, 1, 1, c, e);
    Engine_MapCopyCellAttributes(25, 21, 1, 1, a, e);
    Engine_MapCopyCellAttributes(14, 0x35, 1, 1, b, e);
    f = 0x37;
    Engine_MapCopyCellAttributes(13, 0x37, 1, 1, a, f);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    if (Engine_GameFlagIsSet(0x312) == 0) {
        Engine_EventBegin();
        if (Engine_GameFlagIsSet(0x832) == 0) {
            struct FieldActor *actor = Object_GetById(13);
            struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
            u16 priority = leader->sprite->priority;
            u8 flags = leader->priority_flags;

            Engine_WorkSetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
            Engine_AudioPlayCue(141);
            Engine_TaskWait(40);
            Engine_AudioPlayCue(145);
            ObjectMotion_SetActionVariant(ACTOR_PARTY_LEADER, 3);
            Object_GetById(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            Engine_ActorSetPosition(13, 0, 0x2bf0000);
            actor->speed = 0x18000;
            actor->acceleration = 0x18000;
            actor->y.fixed += 0x500000;
            *(s32 *)((u8 *)actor + 0x3c) = actor->y.fixed;
            *(s32 *)((u8 *)actor + 0x44) = 0x8000;
            Engine_ActorWalkToAndWait(13, 64, 0x2bf);
            Engine_EventWait(40);
            Engine_AudioPlayCue(0x121);
            Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            BattleFx_PlayQueuedSound();
            Engine_GameFlagSet(0x832);
            ObjectMotion_SetActionVariant(ACTOR_PARTY_LEADER, priority);
            Object_GetById(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
            leader->priority_flags = flags;
        }
        SceneState_ApplyFourRects();
        Engine_GameFlagSet(0x312);
        if (Engine_GameFlagIsSet(0x837) != 0) {
            if (Engine_GameFlagIsSet(0x841) == 0) {
                if (Engine_GameFlagIsSet(0x30c) == 0) {
                    if (Object_GetById(ACTOR_PARTY_LEADER)->z.fixed <= 0x2b4ffff) {
                        SceneActor_RunActor22PlacementSequence(62, 0x29d);
                        Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 27, 0x273);
                    } else {
                        SceneActor_RunActor22PlacementSequence(75, 0x2cb);
                        Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 67, 0x2f5);
                    }
                    Engine_GameFlagSet(0x30c);
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

    Engine_MapCopyCellAttributes(29, 22, 1, 1, 3, a);
    b = 2;
    Engine_MapCopyCellAttributes(29, 21, 1, 1, b, a);
    Engine_MapCopyCellAttributes(29, 21, 1, 1, 4, a);
    Engine_MapCopyCellAttributes(23, 20, 3, 1, b, 0x2b);
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

    Engine_MapCopyCellAttributes(29, 20, 1, 1, a, h);
    b = 0x46;
    Engine_MapCopyCellAttributes(29, 20, 1, 1, a, b);
    a2 = 27;
    Engine_MapCopyCellAttributes(29, 20, 1, 1, a2, b);
    Engine_MapCopyCellAttributes(28, 21, 1, 1, 28, h);
    Engine_MapCopyCellAttributes(28, 22, 1, 1, a2, 0x48);
}

void FieldScene_RunScene372SequenceA(void)
{
    s32 base;

    Engine_EventBegin();
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x106, 0x32a);
    Engine_ActorSetPosition(20, 0x1060000, 0x3250000);
    Engine_ActorWalkTo(20, 0x106, 0x339);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x11a, 0x357);
    Engine_ActorSetAnimation(20, 1);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 20, 0);
    BattleFx_PlayQueuedSound();
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorShowEmote(20, 0x100, 20);
    base = (s32)MsgHaidiaHuh;
    Engine_EventSetMessage(base);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(20);
    Engine_EventAskYesNo(20, 0);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventSetMessage((base + 4));
    Engine_EventShowMessageAndWait(20, 0, 20);
    Object_SetActionCallbackAndRefreshById(20, (s32)HaidiaArashi_ActorTwentyScript);
    Engine_GameFlagSet(0x835);
    Engine_EventEnd();
}

void FieldScene_RunScene372SequenceB(void)
{
    u32 i;
    s32 record;

    if (Engine_GameFlagIsSet(0x836) == 0) {
        if (Engine_GameFlagIsSet(0x837) == 0) {
            Engine_EventBegin();
            Engine_EventSetMessage((s32)MsgHaidiaUghHrnghhh);
            Engine_EventShowMessageAndWait(22, 0, 20);
            Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x101, 40);
            Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x17e, 0x26b);
            Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 22, 0);
            Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Engine_EventWait(30);
            Engine_EventShowMessage(22, 0);
            Engine_GameFlagSet(0x836);
            Engine_EventEnd();
        }
    }
}

void FieldScene_RunActor22SceneWhenFlag836Only(void)
{
    if (Engine_GameFlagIsSet(0x837) == 0 && Engine_GameFlagIsSet(0x836) != 0) {
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
    if (Engine_GameFlagIsSet(0x841) != 0) {
        Engine_EventBegin();
        Engine_ActorFaceActor(22, ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgHaidiaBoulderNeedGet);
        Engine_EventShowMessage(22, 0);
        Engine_ActorFaceDirection(22, 0xe000, 10);
        Engine_EventEnd();
    } else {
        if (Engine_GameFlagIsSet(0x837) == 0) {
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

    if (Engine_GameFlagIsSet(0x837) == 0) {
        Engine_EventBegin();
        Engine_ActorSetAttachedEffect(22, 0x100);
        base = (s32)MsgHaidiaHey;
        Engine_EventSetMessage(base);
        Engine_EventShowMessage(22, 0);
        Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x100, 20);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Engine_CameraSetSpeed(0x6666, 0xccc);
        Engine_CameraMoveTo(0x1000000, -1, 0x24c0000, 1);
        Engine_ActorSetSpeed(22, 0x20000, 0x10000);
        Object_SetActionCallbackAndRefreshById(22, (s32)HaidiaArashi_ActorTwentyTwoScriptA);
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
        Engine_EventWait(30);
        ((s32 (*)())Engine_ActorEnableActionCallback)(22, (s32)HaidiaArashi_ActorTwentyTwoScriptB);
        Engine_EventShowMessage(22, 0);
        record = Object_GetById(22);
        *(s32 *)(record + 28) = 0x10000;
        Engine_ActorRunRepeatedMotion(22, 1);
        Engine_EventWait(20);
        Engine_EventAskYesNo(22, 0);
        Engine_EventWait(40);
        Engine_ActorRunRepeatedMotion(22, 1);
        Engine_EventSetMessage((base + 5));
        Engine_EventShowMessageAndWait(22, 0, 20);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimationAndWait(22, 3);
        Engine_EventShowMessage(22, 0);
        Engine_ActorSetSpeed(22, 0x10000, 0x8000);
        Engine_ActorSetAnimation(22, 2);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Engine_ActorSetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(22);
        Engine_ActorSetPosition(22, 0, 0);
        Event_PrepareObjectAndApplyValue(1, 1);
        Engine_ActorSetAnimation(21, 3);
        Engine_GameFlagSet(0x837);
        Engine_EventEnd();
    }
}

void FieldScene_RunScene372SequenceD(void)
{
    u32 i;
    s32 record;
    struct FieldActor *actor;

    Engine_EventBegin();
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Engine_ActorSetPosition(22, actor->x.fixed, actor->z.fixed);
    }
    Engine_ActorSetSpeed(22, 0x10000, 0x8000);
    Engine_ActorWalkToAndWait(22, 0x119, 0x1fb);
    Engine_ActorFaceEachOther(22, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(30);
    Engine_EventSetMessage((s32)MsgHaidiaNorthLeadsToMtAleph);
    Engine_EventShowMessage(22, 0);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 22, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(22, 0x4000, 0);
    Engine_EventShowMessage(22, 0);
    Engine_ActorSetAnimation(22, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Engine_ActorSetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(22);
    Engine_ActorSetPosition(22, 0, 0);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x205);
    Engine_EventEnd();
}

void SceneActor_RunActor22PlacementSequence(s32 x, s32 y)
{
    Thing1 *a;
    s32 w = 0x10000;
    s32 h = 0x8000;
    Thing2 *b;

    a = Object_GetById(ACTOR_PARTY_LEADER);
    if (a != 0) {
        Engine_ActorSetPosition(22, a->unk8, a->unk10);
    }
    Engine_ActorSetSpeed(22, w, h);
    Engine_ActorWalkToAndWait(22, x, y);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
    Engine_EventWait(20);
    Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_EventWait(40);
    Engine_EventSetMessage((s32)MsgHaidiaCantGetAroundThisRock);
    Engine_EventShowMessage(22, 0);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventShowMessage(22, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(22, 2);
    b = Object_GetById(ACTOR_PARTY_LEADER);
    if (b != 0) {
        Engine_ActorSetDestination(22, b->unkA, b->unk12);
    }
    Engine_ActorWaitForMove(22);
    Engine_ActorSetPosition(22, 0, 0);
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

    if (Engine_GameFlagIsSet(FLAG_BOULDER_FELL) == 0) {
        Engine_EventBegin();
        Event_CallWithLastActiveObjectId((s32)HaidiaArashi_LastObjectCall);
        SceneState_SetValue140Mode0();
        Engine_TaskWait(1);
        Engine_AudioPlayCue(141);
        Engine_WorkSetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Engine_EventWait(30);
        Engine_WorkSetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Engine_AudioPlayCue(145);
        Engine_EventWait(30);
        actor = Object_GetById(ACTOR_PARTY_LEADER);
        if (actor != NULL) {
            Engine_ActorSetPosition(22, actor->x.fixed, actor->z.fixed);
        }
        Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
        Engine_ActorSetSpeed(22, 0x20000, 0x10000);
        Engine_ActorEnableActionCallback(0, (s32)HaidiaArashi_LeaderScript);
        Object_SetActionCallbackAndRefreshById(22, (s32)HaidiaArashi_ActorTwentyTwoScript);
        Object_RefreshSelectorById(0);
        Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
        Engine_ActorShowEmote(22, 0x100, 30);
        Engine_WorkSetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Engine_AudioPlayCue(145);
        Engine_EventWait(40);
        Engine_WorkSetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Engine_AudioPlayCue(145);
        Engine_EventWait(20);
        Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Engine_ActorSetAttachedEffect(22, 0x102);
        Engine_EventWait(40);
        Engine_ActorSetAnimation(32, 5);
        Engine_ActorSetAnimation(33, 5);
        Engine_ActorSetAnimation(30, 8);
        Engine_ActorSetAnimation(29, 8);
        Object_GetById(30)->scale_x = -0x10000;
        ObjectMotion_SetActionVariant(32, 2);
        ObjectMotion_SetActionVariant(33, 2);
        ObjectMotion_SetActionVariant(30, 3);
        ObjectMotion_SetActionVariant(29, 3);
        Engine_EventSetMessage((s32)MsgHaidiaTheBoulderIsFalling);
        Engine_EventShowMessageAndWait(28, 0, 20);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Engine_ActorFaceDirection(22, 0xc000, 20);
        Engine_CameraSetSpeed(0x40000, 0x8000);
        Engine_CameraMoveTo(0x700000, -1, 0x14b0000, 1);
        Engine_CameraWaitForMove();
        for (i = 0; i < 40; i++) {
            SceneActor_SetModeByFrameBit1(Object_GetById(32));
            SceneActor_SetModeByFrameBit1(Object_GetById(33));
            SceneActor_SetModeByFrameBit1(Object_GetById(30));
            SceneActor_SetModeByFrameBit1(Object_GetById(29));
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
        Engine_ActorSetPosition(19, 0x720000, 0x1220000);
        record = Object_GetById(19);
        shifted = *(s32 *)(record + 12) + 0x400000;
        *(s32 *)(record + 12) = shifted;
        *(s32 *)(record + 60) = shifted;
        Engine_ActorSetSpeed(19, 0xcccc, 0x6666);
        Engine_AudioPlayCue(145);
        Engine_ActorMoveToAndWait(19, 114, 0x14d);
        Engine_ActorSetAnimation(19, 2);
        Engine_WorkSetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Engine_AudioPlayCue(145);
        *phase = 0;
        Engine_ActorSetSpeed(19, 0x6666, 0x3333);
        Engine_ActorMoveToAndWait(19, 114, 0x12c);
        Engine_ActorSetAnimation(19, 2);
        Engine_WorkSetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Engine_AudioPlayCue(145);
        *phase = 2;
        Engine_ActorSetSpeed(19, 0xcccc, 0x6666);
        Engine_ActorMoveToAndWait(19, 114, 0x14d);
        Engine_ActorSetAnimation(19, 2);
        Engine_WorkSetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Engine_AudioPlayCue(145);
        *phase = 0;
        Engine_ActorSetSpeed(19, 0x6666, 0x3333);
        Engine_ActorMoveToAndWait(19, 114, 0x12c);
        Engine_ActorSetAnimation(19, 2);
        Engine_WorkSetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Engine_AudioPlayCue(145);
        *phase = 2;
        Engine_ActorSetSpeed(19, 0xcccc, 0x6666);
        Engine_ActorMoveToAndWait(19, 114, 0x14d);
        Engine_ActorSetAnimation(19, 2);
        Engine_WorkSetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Engine_AudioPlayCue(145);
        *phase = 1;
        Engine_EventWait(20);
        Engine_ActorSetAttachedEffect(32, 0x102);
        Engine_ActorRunRepeatedMotion(32, 2);
        Engine_EventShowMessage(31, 0);
        Engine_ActorShowEmote(33, 0x100, 0);
        Engine_ActorRunRepeatedMotion(33, 2);
        Engine_EventShowMessageAndWait(28, 0, 40);
        Engine_ActorSetAttachedEffect(30, 0x102);
        Engine_ActorRunRepeatedMotion(30, 2);
        Engine_EventShowMessage(30, 0);
        HaidiaArashi_ShakeDone = 1;
        Engine_ActorSetAnimation(29, 1);
        Engine_TaskWait(1);
        Engine_ActorSetChildValue(29, 0);
        Engine_ActorShowEmote(29, 0x105, 20);
        Engine_ActorFaceDirection(29, 0x8000, 40);
        Engine_ActorFaceDirection(29, 0, 20);
        Engine_ActorFaceDirection(29, 0x8000, 20);
        Engine_ActorFaceDirection(29, 0x4000, 40);
        Engine_ActorShowEmote(29, 0x100, 0);
        Engine_ActorRunRepeatedMotion(29, 2);
        Engine_ActorJump(29, 4, 40);
        Engine_ActorSetAnimation(29, 9);
        Engine_EventWait(10);
        Engine_EventShowMessageAndWait(29, 0, 20);
        Engine_AudioPlayCue(0x121);
        Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_CameraSetSpeed(0x60000, 0xc000);
        Engine_CameraMoveTo(0x540000, -1, 0x2340000, 1);
        Engine_CameraWaitForMove();
        BattleFx_PlayQueuedSound();
        Engine_ActorFaceActor(22, ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(20);
        Engine_ActorSetAttachedEffect(22, 0x102);
        Engine_EventWait(30);
        Scheduler_RemoveCallback(steps);
        Scheduler_RemoveCallback(callback);
        Engine_EventShowMessage(22, 0);
        Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 22, 0);
        Engine_EventWait(20);
        FieldScene_RunSingleStep();
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimationAndWait(22, 3);
        Engine_EventWait(20);
        Engine_ActorSetAnimation(22, 2);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Engine_ActorSetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(22);
        Engine_ActorSetPosition(22, 0, 0);
        Engine_ActorDestroy(31);
        Engine_ActorDestroy(28);
        Engine_ActorDestroy(30);
        Engine_ActorDestroy(29);
        Engine_ActorDestroy(32);
        Engine_ActorDestroy(33);
        Engine_GameFlagSet(FLAG_BOULDER_FELL);
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
        OverlayObject_SetChildByte5AndMark(Object_GetById(32), 1);
        OverlayObject_SetChildByte5AndMark(Object_GetById(33), 1);
        OverlayObject_SetChildByte5AndMark(Object_GetById(30), 1);
        OverlayObject_SetChildByte5AndMark(Object_GetById(29), 1);
    } else {
        OverlayObject_SetChildByte5AndMark(Object_GetById(32), 8);
        OverlayObject_SetChildByte5AndMark(Object_GetById(33), 8);
        OverlayObject_SetChildByte5AndMark(Object_GetById(30), 8);
        OverlayObject_SetChildByte5AndMark(Object_GetById(29), 8);
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

    Engine_ActorFaceDirection(26, 0x3000, 0);
    Engine_ActorFaceDirection(24, 0xd000, 0);
    Engine_ActorFaceDirection(25, 0xb000, 0);
    Engine_ActorFaceDirection(9, 0x3000, 0);
    Engine_ActorFaceDirection(10, 0xd000, 20);
    Engine_ActorSetAnimation(26, 3);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimation(25, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(25, 3);
    Engine_EventWait(20);
    Engine_CameraSetSpeed(0x10000, 0x2000);
    Engine_CameraMoveTo(0x860000, -1, 0x4ab0000, 1);
    Engine_ActorSetSpeed(26, 0x19999, 0xcccc);
    Engine_ActorSetSpeed(9, 0x19999, 0xcccc);
    Engine_ActorEnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptA);
    Object_SetActionCallbackAndRefreshById(9, (s32)HaidiaArashi_ActorNineScriptA);
    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells((const u16 *)HaidiaArashi_CellSteps2, 38, 72);
    Engine_EventWait(10);
    Engine_ActorWalkToAndWait(9, 149, 0x497);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorWalkToAndWait(25, 250, 0x4be);
    BattleFx_PlayQueuedSound();
    Engine_ActorFaceDirection(10, 0x3000, 0);
    Engine_ActorFaceDirection(24, 0x3000, 0);
    Engine_ActorFaceDirection(25, 0x3000, 0);
    Engine_ActorSetAnimation(10, 5);
    Engine_ActorSetAnimation(24, 6);
    Engine_ActorSetAnimation(25, 6);
    /* For records 10, 24 and 25: fetch the record's entry pointer, fetch a
     * value from that record's own state, and store a derived value into
     * the entry's field at offset 100. */
    entry = (s32)Object_GetById(10);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (Math_RemainderUnsigned(record, 90) + 60);
    entry = (s32)Object_GetById(24);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (Math_RemainderUnsigned(record, 90) + 60);
    entry = (s32)Object_GetById(25);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (Math_RemainderUnsigned(record, 90) + 60);
    script = (s32)HaidiaArashi_ActorEightScript;
    Engine_ActorEnableActionCallback(10, (const u8 *)script);
    Engine_ActorEnableActionCallback(24, (const u8 *)script);
    Engine_ActorEnableActionCallback(25, (const u8 *)script);
    Object_RefreshSelectorById(26);
    Engine_EventWait(10);
    Engine_AudioPlayCue(159);
    Engine_MapAnimateCells((const u16 *)HaidiaArashi_CellSteps5, 38, 72);
    Engine_EventWait(30);
    BattleFx_PlayQueuedSound();
    Engine_CameraMoveTo(0x700000, -1, 0x4c90000, 1);
    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells((const u16 *)HaidiaArashi_CellSteps1, 35, 73);
    Engine_EventWait(20);
    BattleFx_PlayQueuedSound();
    Engine_ActorEnableActionCallback(9, HaidiaArashi_ActorNineScriptB);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptB);
    Engine_EventWait(40);
    Engine_AudioPlayCue(159);
    Engine_MapAnimateCells((const u16 *)HaidiaArashi_CellSteps4, 35, 73);
    Object_RefreshSelectorById(26);
    BattleFx_PlayQueuedSound();
    Engine_EventWait(40);
    north = (s32)MsgHaidiaGoLookNorth;
    Engine_EventSetMessage(north);
    Engine_EventShowMessageAndWait(9, 0, 20);
    Engine_ActorSetAnimationAndWait(26, 3);
    Engine_EventShowMessageAndWait(0x201a, 0, 40);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(26, 3);
    Engine_EventWait(30);
    Engine_ActorEnableActionCallback(9, HaidiaArashi_ActorNineScriptC);
    Engine_ActorEnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptC);
    Engine_EventWait(40);
    Engine_CameraSetSpeed(0x20000, 0x4000);
    Engine_CameraMoveTo(0x690000, -1, 0x43e0000, 1);
    Object_RefreshSelectorById(9);
    Engine_ActorFaceDirection(9, 0, 0);
    Engine_ActorShowEmote(9, 0x100, 40);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Engine_ActorFaceDirection(22, 0x8000, 10);
    Engine_ActorWalkToAndWait(9, 105, 0x43e);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventOpenMessage(0x8009, 0);
    Engine_ActorFaceDirection(22, 0, 0);
    /* Branch on a condition; pass byte 4 or byte 5 of the 0xe9b
     * table to the corresponding follow-up call. */
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(9, 3);
        Engine_EventSetMessage((north + 4));
    } else {
        Engine_ActorRunRepeatedMotion(9, 2);
        Engine_EventSetMessage((north + 5));
    }
    Engine_EventShowMessage(0x8009, 0);
    Engine_ActorFaceDirection(22, 0x8000, 40);
    Engine_ActorShowEmote(9, 0x100, 30);
    suppose = (s32)MsgHaidiaDontSupposeTwo;
    Engine_EventSetMessage(suppose);
    Engine_EventOpenMessage(0x8009, 0);
    /* Branch on a condition; each side reads a different byte of the
     * 0xea1 table and runs its own follow-up sequence. */
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(9, 3);
        Engine_EventSetMessage((suppose + 1));
        Engine_EventShowMessageAndWait(0x8009, 0, 30);
        Engine_ActorFaceDirection(22, 0x8000, 20);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimation(22, 3);
        Engine_ActorSetAnimationAndWait(9, 3);
        Engine_EventWait(40);
    } else {
        Engine_ActorShowEmote(9, 0x105, 90);
        Engine_ActorShowEmote(9, 0x103, 40);
        Engine_ActorSetAnimation(9, 4);
        Engine_EventSetMessage((suppose + 2));
        Engine_EventShowMessage(0x8009, 0);
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
    record = (s32)Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(22);
    Engine_ActorSetPosition(22, 0, 0);
}

void SceneDialogue_RunActorTenFlag30dDialogue(void)
{
    s32 v2000 = 0x2000;
    u8 *tbl;

    Engine_EventBegin();
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 20);
    if (Engine_GameFlagIsSet(0x30d) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaTwoDontEnough);
        Engine_EventShowMessageAndWait(10, 0, 10);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaOh);
        Engine_ActorStartRepeatedMotion(10, 1);
        Engine_EventShowMessageAndWait(10, 0, 10);
        Engine_ActorStartRepeatedMotion(10, 2);
        Engine_EventShowMessageAndWait(10, 0, 10);
    }
    Engine_ActorFaceDirection(10, v2000, 20);
    Engine_ActorSetAnimation(10, 5);
    Engine_EventWait(10);
    {
        u8 *rec;
        s32 v;
        rec = Object_GetById(10);
        v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
        tbl = HaidiaArashi_ActorEightScript;
        *(u16 *)(rec + 0x64) = v;
        Engine_ActorEnableActionCallback(10, tbl);
    }
    Engine_EventWait(20);
    Engine_GameFlagSet(0x30d);
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
extern const u8 HaidiaArashi_ArriveScript[];
extern const u8 HaidiaArashi_GatherPath8[];
extern const u8 HaidiaArashi_GatherPath26[];
extern const u8 HaidiaArashi_GatherPath0[];
extern const u8 HaidiaArashi_GatherPath22[];
extern const u8 HaidiaArashi_LookAroundScript[];
extern const u8 HaidiaArashi_ActorNineteenRiseScript[];
struct FieldActor *Battle_GetWorkObject1e0(void);
void HaidiaArashi_UpdatePulsingGlow(void);
void ActorPresentation_SelectActorTwentySevenState(void);
void OverlayObject_CopyRecordField1ToSlots22And8(void);

/* Sets an actor and its target at once: x and height from one value, depth
   from the other. */
static __inline__ void HaidiaArashi_PlaceActor(struct FieldActor *object, s32 pos, s32 depth)
{
    object->x.fixed = pos;
    object->y.fixed = pos;
    object->target_x = pos;
    object->target_y = pos;
    object->z.fixed = depth;
    object->target_z = depth;
}

/* The night of the storm, after the Boulder: the group gathers, the light
   rises and bursts, and the scene is left set for the morning after. */
void Scene_RunActorGroupDepartureSequence(void)
{
    struct FieldSprite *actorVisual;
    struct FieldSprite *groupVisual;
    struct FieldActor *actor;
    struct FieldActor *fieldActor;
    struct FieldActor *groupActor;
    struct FieldActor *work;
    u32 random;
    const u8 *entryActions;
    s32 zero;
    u8 still;
    const u8 *moveActions;
    s32 phase;
    const u8 *departureActions;

    actor = Object_GetById(19);
    groupActor = Object_GetById(27);
    actorVisual = actor->sprite;
    groupVisual = groupActor->sprite;
    Engine_CameraSetSpeed(0x10000, 0x2000);
    Engine_CameraMoveTo(0x6e0000, -1, 0x58b0000, 1);
    Engine_ActorSetSpeed(8, 0x13333, 0x9999);
    Engine_ActorSetSpeed(26, 0x13333, 0x9999);
    Engine_ActorSetSpeed(0, 0x13333, 0x9999);
    Engine_ActorSetSpeed(22, 0x13333, 0x9999);
    entryActions = HaidiaArashi_ArriveScript;
    Engine_ActorEnableActionCallback(8, entryActions);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(26, entryActions);
    BattleFx_SetBlock30Values12Zero();
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(0, entryActions);
    Engine_EventWait(10);
    BattleFx_SetBlock30ValuesMaxZero();
    Engine_ActorEnableActionCallback(22, entryActions);
    Engine_EventWait(128);
    SceneState_SetFlag210AndConfigureRegion40_89();
    Engine_CameraMoveTo(0xae0000, -1, 0x5940000, 1);
    Engine_EventWait(104);
    Engine_CameraMoveTo(0x990000, -1, 0x52d0000, 1);
    Engine_ActorWalkToAndWait(9, 158, 0x4f8);
    Engine_ActorFaceDirection(9, 0x2000, 0);
    Object_RefreshSelectorById(8);
    Engine_ActorEnableActionCallback(8, HaidiaArashi_GatherPath8);
    Engine_ActorEnableActionCallback(26, HaidiaArashi_GatherPath26);
    Engine_ActorEnableActionCallback(0, HaidiaArashi_GatherPath0);
    Object_SetActionCallbackAndRefreshById(22, HaidiaArashi_GatherPath22);
    Engine_WorkSetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(20);
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Engine_EventWait(60);
    Engine_ActorShowEmote(0, 0x101, 0);
    Engine_ActorShowEmote(26, 0x101, 0);
    Engine_ActorShowEmote(22, 0x101, 0);
    Engine_ActorShowEmote(8, 0x101, 0);
    Engine_ActorShowEmote(9, 0x101, 60);
    Engine_ActorFaceEachOther(26, 8, 0);
    Engine_ActorFaceEachOther(22, 0, 0);
    Engine_EventWait(20);
    fieldActor = Object_GetById(0);
    random = Engine_RandomNext();
    random = Math_RemainderUnsigned(random, 20) + 20;
    fieldActor->unknown_64 = random;
    zero = 0;
    still = 0;
    fieldActor = Object_GetById(22);
    random = Engine_RandomNext();
    fieldActor->unknown_64 = (Math_RemainderUnsigned(random, 20) + 20);
    fieldActor = Object_GetById(26);
    random = Engine_RandomNext();
    fieldActor->unknown_64 = (Math_RemainderUnsigned(random, 20) + 20);
    fieldActor = Object_GetById(8);
    random = Engine_RandomNext();
    fieldActor->unknown_64 = (Math_RemainderUnsigned(random, 20) + 20);
    fieldActor = Object_GetById(9);
    random = Engine_RandomNext();
    fieldActor->unknown_64 = (Math_RemainderUnsigned(random, 20) + 20);
    moveActions = HaidiaArashi_LookAroundScript;
    Engine_ActorEnableActionCallback(9, moveActions);
    Engine_EventWait(30);
    Engine_ActorEnableActionCallback(0, moveActions);
    Engine_ActorEnableActionCallback(26, moveActions);
    Engine_ActorEnableActionCallback(22, moveActions);
    Engine_ActorEnableActionCallback(8, moveActions);
    Engine_EventWait(10);
    Engine_AudioPlayCue(17);
    Engine_WorkSetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(30);
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Engine_EventWait(120);
    Engine_WorkSetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(40);
    Engine_WorkSetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Engine_EventWait(60);
    Engine_WorkSetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(20);
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Engine_EventWait(60);
    Engine_WorkSetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(40);
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Engine_EventWait(60);
    BattleFx_SetBlock30ValuesMaxZero();
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Engine_EventWait(1);
    Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_CameraSetSpeed(0x80000, 0x80000);
    Engine_CameraMoveTo(0xd90000, -1, 0x43c0000, 1);
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(40);
    Engine_TaskWait(40);
    Engine_ActorSetChildValue(19, 0);
    work = Object_GetById(19);
    Engine_ActorSetSpriteFlags(work, 0);
    work = Object_GetById(27);
    Engine_ActorSetSpriteFlags(work, 0);
    groupActor->scale_x = 0xcccc;
    groupActor->scale_y = 0xcccc;
    groupActor->priority_flags &= 254;
    groupVisual->priority = 1;
    HaidiaArashi_PlaceActor(actor, 0xc80000, 0x3820000);
    actor->motion_flags = still;
    actor->priority_flags &= 254;
    actorVisual->priority = 0;
    work = Battle_GetWorkObject1e0();
    work->target_x = 0x80000000;
    work = Battle_GetWorkObject1e0();
    work->target_y = 0x80000000;
    work = Battle_GetWorkObject1e0();
    work->target_z = 0x80000000;
    work = Battle_GetWorkObject1e0();
    work->velocity_x = zero;
    work = Battle_GetWorkObject1e0();
    work->velocity_y = zero;
    work = Battle_GetWorkObject1e0();
    work->velocity_z = zero;
    Engine_TaskWait(1);
    Engine_CameraMoveTo(0xf70000, 0x800000, 0x3950000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_ColorBufferApplyTarget(0x10003, 1);
    Engine_ColorBufferApplyTarget(0x10000, 2);
    Engine_ColorBufferInterpolate(30);
    Engine_TaskWait(30);
    Scheduler_AddOrUpdateCallback(HaidiaArashi_UpdatePulsingGlow, 0xc80);
    Engine_ActorEnableActionCallback(19, HaidiaArashi_ActorNineteenRiseScript);
    Engine_CameraSetSpeed(0x20000, 0x7ae);
    Engine_CameraMoveTo(0xaf0000, 0x600000, 0x43e0000, 1);
    do {
        Engine_TaskWait(1);
    } while ((s16)actor->unknown_66 != 8);
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    Engine_MapWaitWorkValuesBelow256();
    work = Battle_GetWorkObject1e0();
    work->target_x = 0x80000000;
    work = Battle_GetWorkObject1e0();
    work->target_y = 0x80000000;
    work = Battle_GetWorkObject1e0();
    work->target_z = 0x80000000;
    phase = 0;
    work = Battle_GetWorkObject1e0();
    work->velocity_x = phase;
    work = Battle_GetWorkObject1e0();
    work->velocity_y = phase;
    work = Battle_GetWorkObject1e0();
    work->velocity_z = phase;
    Scheduler_RemoveCallback(HaidiaArashi_UpdatePulsingGlow);
    Engine_ActorStop(19);
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(19, 0);
    groupActor->scale_x = 0x14000;
    groupActor->scale_y = 0x14000;
    groupVisual->unknown_20[3] = 2;
    groupVisual->scale = 0x14000;
    actor->scale_x = 0x20000;
    actor->scale_y = 0x20000;
    actor->x.fixed = phase;
    actor->z.fixed = phase;
    actor->target_x = phase;
    actor->target_z = phase;
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(23, 8);
    Engine_ActorSetPosition(9, 0xa90000, 0x4f00000);
    Engine_ActorFaceDirection(9, 0xc000, 0);
    Engine_ActorSetAnimation(9, 9);
    Engine_ActorSetPosition(26, 0x970000, 0x50c0000);
    Engine_ActorFaceDirection(26, 0x8000, 0);
    Engine_ActorSetAnimation(26, 5);
    Engine_ActorSetPosition(8, 0xaa0000, 0x5210000);
    Engine_ActorFaceDirection(8, 0x6000, 0);
    Engine_ActorSetAnimation(8, 5);
    Engine_ActorSetPosition(0, 0xb90000, 0x5350000);
    Engine_ActorFaceDirection(0, 0x2000, 0);
    Engine_ActorSetAnimation(0, 17);
    Engine_ActorSetPosition(22, 0xa90000, 0x5680000);
    Engine_ActorFaceDirection(22, 0x4000, 0);
    Engine_ActorSetAnimation(22, 0);
    Engine_CameraMoveTo(0xa60000, 0, 0x5390000, 0);
    Engine_MapRedraw();
    actor->motion_flags = phase;
    actor->target_x = 0x80000000;
    actor->target_y = 0x80000000;
    actor->target_z = 0x80000000;
    HaidiaArashi_FlashLightning();
    Engine_ActorSetPosition(27, 0xda0000, 0x4980000);
    Engine_CameraMoveTo(0xd20000, 0, 0x4ac0000, 0);
    Engine_MapRedraw();
    groupActor->scale_x = 0x20000;
    groupActor->scale_y = 0x20000;
    Scheduler_AddOrUpdateCallback(ActorPresentation_SelectActorTwentySevenState, 0xc80);
    Engine_ActorStop(10);
    Engine_ActorStop(24);
    Engine_ActorStop(25);
    Engine_TaskWait(1);
    groupActor = Object_GetById(10);
    groupVisual = groupActor->sprite;
    groupActor->priority_flags &= 254;
    groupActor->scale_x = 0x10000;
    groupActor->scale_y = 0x10000;
    groupActor->facing = 0xd000;
    groupVisual->priority = 0;
    Engine_ActorSetAnimation(10, 0);
    groupActor = Object_GetById(24);
    groupVisual = groupActor->sprite;
    groupActor->priority_flags &= 254;
    groupActor->scale_x = 0x10000;
    groupActor->scale_y = 0x10000;
    groupActor->facing = 0xb000;
    groupVisual->priority = 0;
    Engine_ActorSetAnimation(24, 5);
    groupActor = Object_GetById(25);
    groupVisual = groupActor->sprite;
    groupActor->priority_flags &= 254;
    groupActor->scale_x = 0x10000;
    groupActor->scale_y = 0x10000;
    groupActor->facing = 0xb000;
    groupVisual->priority = 0;
    Engine_ActorSetAnimation(25, 5);
    groupActor = Object_GetById(27);
    groupVisual = groupActor->sprite;
    HaidiaArashi_FlashLightning();
    actor->y.fixed = 0x300000;
    actor->x.fixed = 0xd60000;
    actor->z.fixed = 0x4c00000;
    actor->target_x = 0x80000000;
    actor->target_y = 0x80000000;
    actor->target_z = 0x80000000;
    groupVisual->priority = 1;
    Engine_ActorSetPosition(27, 0xd60000, 0x4c00000);
    Engine_ActorFaceDirection(24, 0xc000, 0);
    Engine_ActorFaceDirection(25, 0xc000, 20);
    Engine_GameFlagSet(0x166);
    Map_SetLayerEntryFlag(0);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Map_SetLayerEntryFlag(3);
    Map_SetLayerEntryFlag(4);
    Map_SetLayerEntryFlag(5);
    Engine_ColorBufferApplyTarget(0x10003, 1);
    Engine_ColorBufferApplyTarget(0x10000, 2);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(160);
    Engine_ColorBufferApplyTarget(0x7fff, 1);
    Engine_ColorBufferApplyTarget(0x7fff, 2);
    Engine_ColorBufferInterpolate(80);
    Engine_EventWait(80);
    Engine_EventWait(100);
    Scheduler_RemoveCallback(ActorPresentation_SelectActorTwentySevenState);
    groupVisual->scale = groupActor->scale_x;
    Engine_GameFlagClear(0x166);
    Map_ClearLayerEntryFlag(0);
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Map_ClearLayerEntryFlag(3);
    Map_ClearLayerEntryFlag(4);
    Map_ClearLayerEntryFlag(5);
    FieldScene_BuildPlacementGrid();
    Engine_ActorSetPosition(9, 0xa50000, 0x4cd0000);
    Engine_ActorSetAnimation(9, 1);
    actor = Object_GetById(9);
    actor->facing = 0xe000;
    random = Engine_RandomNext();
    actor->unknown_64 = Math_RemainderUnsigned(random, 90) + 60;
    departureActions = HaidiaArashi_ActorEightScript;
    actor->unknown_66 = 1;
    Engine_ActorEnableActionCallback(9, departureActions);
    Engine_ActorSetPosition(26, 0xa50000, 0x4e60000);
    Engine_ActorSetAnimation(26, 1);
    actor = Object_GetById(26);
    actor->facing = 0xe000;
    random = Engine_RandomNext();
    actor->unknown_64 = Math_RemainderUnsigned(random, 90) + 60;
    actor->unknown_66 = 2;
    Engine_ActorEnableActionCallback(26, departureActions);
    Engine_ActorSetPosition(22, 0x980000, 0x5050000);
    Engine_ActorSetAnimation(22, 1);
    actor = Object_GetById(22);
    actor->facing = 0xe000;
    random = Engine_RandomNext();
    actor->unknown_64 = Math_RemainderUnsigned(random, 90) + 60;
    actor->unknown_66 = 3;
    Engine_ActorEnableActionCallback(22, departureActions);
    Engine_ActorSetPosition(8, 0xb40000, 0x51f0000);
    actor = Object_GetById(8);
    actor->facing = 0xe000;
    random = Engine_RandomNext();
    actor->unknown_64 = Math_RemainderUnsigned(random, 90) + 60;
    actor->unknown_66 = 4;
    Engine_ActorEnableActionCallback(8, departureActions);
    Engine_ActorSetAnimation(8, 6);
    Object_GetById(22)->priority_flags &= 254;
    Object_GetById(8)->priority_flags &= 254;
    Scheduler_AddOrUpdateCallback(OverlayObject_CopyRecordField1ToSlots22And8, 0xc80);
    Engine_ActorSetPosition(0, 0xb50000, 0x4f90000);
    work = Object_GetById(0);
    work->facing = 0xe000;
    Engine_ActorSetAnimation(0, 1);
    Engine_CameraMoveTo(0xb50000, 0, 0x4f90000, 0);
    Engine_MapRedraw();
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(19, 0, 0);
    Engine_ActorSetPosition(24, 0, 0);
    Engine_ActorSetPosition(25, 0, 0);
    Engine_ActorSetPosition(23, 0, 0);
    Engine_ActorSetPosition(27, 0, 0);
    Engine_ActorSetPosition(17, 0x900000, 0x42e0000);
    Engine_ActorSetPosition(18, 0x1140000, 0x4f60000);
    Engine_TaskWait(60);
    Engine_ColorBufferApplyTarget(0x10003, 1);
    Engine_ColorBufferApplyTarget(0x10000, 2);
    Engine_ColorBufferInterpolate(80);
    Engine_EventWait(60);
    BattleFx_PlayQueuedSound();
    Engine_EventWait(60);
    Party_RemoveOwnerRestored(1);
    BattleFx_SetBlock30Values128One();
}
