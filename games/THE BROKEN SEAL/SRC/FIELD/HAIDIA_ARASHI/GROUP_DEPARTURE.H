#ifndef ALCHEMY_FIELD_HAIDIA_ARASHI_GROUP_DEPARTURE_H
#define ALCHEMY_FIELD_HAIDIA_ARASHI_GROUP_DEPARTURE_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* Set once the Boulder has come down. */
enum {
    FLAG_BOULDER_FELL = 0x838
};

/*
 * Robin's parents, in the scene where the Boulder starts to slip: Dora asks
 * Kyle whether it can be stopped, and he answers her by name.
 */
enum StormNightActor {
    ACTOR_DORA = ACTOR_FIRST_PLACED + 1,
    ACTOR_KYLE
};


#define ACTOR_ID 22

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[92];
    s16 counter;
};

typedef struct Obj {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[4];
    s32 f18;
    s32 f1c;
    u8 pad20[0x44];
    u16 f64;
    u8 pad66[2];
    struct Obj *f68;
} Obj;

typedef struct Ent {
    u8 pad00[9];
    u8 b01:2;
    u8 f:2;
    u8 b45:4;
} Ent;

typedef struct Rec {
    u8 pad00[0x50];
    Ent *f50;
} Rec;

struct FixedPointCountdown {
    u8 pad_00[0x18];
    s32 fixed_point_18;
    s32 fixed_point_1c;
    u8 pad_20[0x44];
    s16 countdown;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Thing1;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} Thing2;

struct Actor {
    u8 reserved_00[80];
    u8 *presentation;
};

extern u8 HaidiaArashi_FrameModes[];

extern u8 HaidiaArashi_SceneTable0[];

extern u8 HaidiaArashi_SceneTable1[];

extern u8 HaidiaArashi_SceneTable2[];

extern u8 HaidiaArashi_SceneTable3[];

extern u8 HaidiaArashi_CellSteps0[];

extern u8 HaidiaArashi_CellSteps1[];




extern u8 HaidiaArashi_ActorNineteenScript[];

extern u8 HaidiaArashi_ActorEightScript[];



/* The import veneer the actors' sprite priority goes through. */
void ObjectMotion_SetActionVariant(s32 actor, s32 priority);

extern s32 HaidiaArashi_ShakeDone;
extern s32 HaidiaArashi_ShakeShift;
/* The scene's tables, cell steps and action scripts, in the overlay's read-only data. */
extern u8 HaidiaArashi_CellSteps2[];
extern u8 HaidiaArashi_CellSteps3[];
extern u8 HaidiaArashi_LastObjectCall[];
extern u8 HaidiaArashi_LeaderScript[];
extern u8 HaidiaArashi_ActorTwentyScript[];
extern u8 HaidiaArashi_ActorTwentyTwoScript[];

s32 IwramUnsignedRemainder(s32, s32);

void HaidiaArashi_SpawnEffectPair(s32);

void BattleFx_SetQueuedSoundAndPlay(s32);

void Effect_SoundAndFlash(void);

void BattleFx_StartTwelveFrameBlend(void);

void BattleFx_SetBlock30Values128One(void);

void HaidiaArashi_RunCallOutSequence(void);

void BattleFx_SetBlock30ValuesMaxZero(void);

s32 BattleFx_SetWeightedResult(s32, s32);

void OverlayObject_ApplyIwramWord1e40(s32 o);

void BattleFx_PlayQueuedSound();

void Event_CallWithLastActiveObjectId();

void Object_RefreshSelectorById();

void Object_SetActionCallbackAndRefreshById();

s32 Scheduler_AddOrUpdateCallback();

void Scheduler_RemoveCallback();

void Map_SetLayerEntryFlag();

void Map_ClearLayerEntryFlag();

void OverlayObject_UpdateRandomSlotByFrame(s32 obj);

void BattleEffect_CleanupSceneObjects(void);

void SceneActor_RunActor22PlacementSequence(s32 x, s32 y);

/* FAKEMATCH: call sites spelled through these wrappers pass their constants
 * straight into the argument registers; a direct call precomputes a costly
 * constant into a pseudo that the compiler then shares with later uses in
 * the block. A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 OverlayObject_SetField6OnCountdown(struct Object *object);
s32 UpdateFixedPointCountdown(struct FixedPointCountdown *state);
u8 *SceneData_GetTableD0E4(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetTableD27c(void);
u8 *SceneData_GetTableD2B8(void);
u8 *SceneData_GetTableD558(void);
void SceneState_SetFlag210AndConfigureRegion40_84(void);
void SceneState_SetFlag210AndConfigureRegion40_89(void);
void SceneState_SetWork1c0AndRunObject(u8 *o);
void FieldScene_SetupDescriptorD774(void);
void SceneState_SetValue123Mode1(void);
void SceneState_ApplyValues123And3(void);
void SceneState_SetValue123Mode4(void);
void FieldScene_RunStep7BAndCheckFlags841And842(void);
void FieldScene_SetupDescriptorD78a(void);
void FieldScene_RunScene372_02000278(void);
void FieldScene_SetupDescriptorD78aIfFlag205Clear(void);
void FieldScene_SetupWithDescriptorD7A0(void);
void FieldScene_SetupDescriptorD7b6(void);
void FieldScene_RunScene372_02000398(void);
void FieldScene_RunScene372_020003cc(void);
void FieldScene_RunScene372_02000400(void);
s32 FieldScene_RunFlagGatedActorSetup(void);
void Scene_DoraSendsRobinToThePlaza(void);
void HaidiaArashi_SetStormCellAttributes(void);
void ActorPresentation_SetEightSceneCells(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void SceneState_ApplyFourRects(void);
void FieldScene_DrawFiveTileBlocks(void);
void FieldScene_RunScene372SequenceA(void);
void FieldScene_RunScene372SequenceB(void);
void FieldScene_RunActor22SceneWhenFlag836Only(void);
void FieldScene_RunScene372SequenceD(void);
void Scene_BoulderFalls(void);
void OverlayObject_SetChildByte5AndMark(u8 *o, s32 n);
void ActorPresentation_SetFourActorsModeByBit(void);
void SceneDialogue_RunActorTenFlag30dDialogue(void);
void HaidiaArashi_FlashLightning(void);
void ActorPresentation_SelectActorTwentySevenState(void);
void FieldScene_RunScene372_02003e48(void);
void FieldScene_ConfigureActorTwentyTwoScene(void);
void FieldScene_BuildPlacementGrid(void);
void SceneState_SetWords1c0And1c8AndRun(void);
void SceneState_SetWorkWordsAndFlag87f(void);
void SceneActor_SetModeByFrameBit1(s32 o);
void SceneEffect_UpdateArcOverAnchor(Obj *o);
void OverlayObject_UpdateArcFromParent(Obj *o);
void SceneState_SetValue140Mode0(void);
void FieldScene_RunSingleStep(void);
void FieldScene_RunFourPairedSteps(void);
void SceneState_SetValue19ThenCall(void);
void OverlayObject_CopyRecordField1ToSlots22And8(void);
void SceneState_SetValueEe4(void);

#endif
