/* Declarations shared by Altin Peak's set-piece sources (resource_3a4). */
#ifndef YAMA_H
#define YAMA_H

#include "TYPES.H"

#define NULL ((void *)0)

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern struct MapRenderWork *gMapWork;

enum {
    /* Message 0x182 + 202. */
    ITEM_FROST_JEWEL = 202
};

enum SetPieceMessage {
    MSG_ROBIN_FLIPPED_SWITCH = 0x1528
};

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

/* The shared work pointer. */
extern u8 Data_03001ebc[];

/* Tables and action scripts in the overlay's data. */
extern u8 ArutinYama_StatueTable[];
extern u8 ArutinYama_EarlyActorScript[];
extern u8 ArutinYama_LogRideScript[];
extern u8 ArutinYama_LeaderRideScript[];
extern u8 ArutinYama_SparkScript[];
extern u8 ArutinYama_GeraldScript[];
extern u8 ArutinYama_IvanScript[];
extern u8 ArutinYama_MiaScript[];
extern u8 ArutinYama_CelebrateScript[];
extern u8 ArutinYama_PartyScript[];
extern u8 ArutinYama_OpenedAreaScript[];
/* Frames counted towards the next sparkle cue. */
extern s32 ArutinYama_CueTicks;

/* Cells after the overlay's image. */
extern s32 ArutinYama_LeafMode;
extern s32 ArutinYama_LeafOrigin[3];

/* Main-image code the import veneers reach. */
s32 ArcTan2(s32, s32);
s32 IwramUnsignedRemainderEntry();
void Runtime_SetIrqHandler();
void SetMapCellCollision();
s32 Object_SetActionCallbackAndRefreshById();
s32 Battle_GetWorkObject1e0();
void Event_LoadAreaScript();
void BattleFx_SetPhaseRequest(s32, s32);
void BattleFx_SetWeightedResult(s32, s32);
void Party_SetFields1ceAnd1d0(s32, s32);
void BattleEffect_CleanupSceneObjects();
void Audio_PlayCueFromEventWork();
s32 BattleFx_SetQueuedSoundAndPlay();
void BattleFx_PlayQueuedSound();

/* FAKEMATCH: call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* The set-piece sources call one another across files. */
void SceneState_SetValue14Mode23(void);
s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *object);
s32 SceneActor_CopyActor8PositionWithFixedY(struct Frame *dst);
s32 OverlayObject_IntegrateAndDamp(u8 *p);
u8 *SceneData_GetTableC85c(void);
void FieldScene_RunSharedSetPiece(s32 a0);
void FieldScene_RunFallingRocksWarning(void);
void SceneState_SetByte22bTo3AndSend51(void);
void SceneActor_UpdateSlot10ByTileX(void);
void ActorPresentation_SetCellAndLowerActorEight(void);
void SceneState_ApplyRectAndSetActor9Byte55(void);
void SceneActor_RaiseSlot9StepA(void);
void SceneActor_RaiseSlot9StepB(void);
void SceneState_ApplyRectAndLowerActor9(void);
void SceneActor_RaiseSlot11AndSetFlag201(void);
void SceneActor_AdjustSlot12AndSetFlag204(void);
void SceneState_SetDispcntBit9ByThreshold(void);
void FieldScene_RunEarlySequence(void);
void FieldScene_RunScene3a4SequenceH(void);
void FieldScene_RunScene3a4SequenceI(void);
void FieldScene_RunScene3a4SequenceB(void);
void FieldScene_RunValue1528Scene(void);
void FieldScene_RunScene3a4SequenceA(void);
void FieldScene_RunLine1528Sequence(void);
void FieldScene_RunScene3a4SequenceC(void);
void FieldScene_RunScene3a4SequenceD(void);
void SceneActor_SetActor10Byte23To3(void);
void SceneActor_SetActor10Byte23To1(void);
void FieldScene_RunScene3a4_02000c9c(void);
void FieldScene_RunScene3a4_02002310(void);
void ArutinYama_UpdateDriftingLeaf(struct FieldActor *object);
void FieldScene_RunScene3a4_02002428(void);
void FieldScene_RunScene3a4_02002490(void);
void FieldScene_RunScene3a4_020025c0(void);
void FieldScene_RunScene3a4_020026c0(void);
void FieldScene_RunScene3a4_02002934(void);
void FieldScene_RunScene3a4_020029dc(void);
void SceneActor_SetMode3AndRate4ccc(u8 *rec);
void SceneActor_SetActor8ModeByCounterBit(void);
void RunEventScript01(void);
void SceneEffect_SpawnObject222(void);
void SceneState_StoreParamsAndInstallTask(s32 v0, s32 v1, s32 v2, s32 v3);
void FieldScene_RunScene3a4SequenceG(void);
void SceneState_ForwardByRuntimeSelector(s32 arg);
void FieldScene_RunActorTenFourStepSequence(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunScene3a4SequenceF(void);
void FieldScene_RunScene3a4SequenceE(void);
void SceneAudio_PlayCue183EverySixtyTicks(void);
void SceneActor_SetActor12ModeByActorZeroHeight(void);
void SceneActor_ClearCollisionFlagAndPlaceMarker(s32 no);
void SceneState_SetRecordWord102AndPlayCue288(u16 *record);

#endif
