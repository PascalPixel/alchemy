#include "GLOBAL_CELLS.H"
#ifndef YAMA_H
#define YAMA_H

#include "TYPES.H"

#define NULL ((void *)0)

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "BATTLE_EFFECT_RUNTIME.H"

enum {
    ITEM_FROST_JEWEL = 202
};

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};


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
extern s32 ArutinYama_CueTicks;

extern s32 ArutinYama_LeafMode;
extern s32 ArutinYama_LeafOrigin[3];

s32 ArcTan2(s32, s32);
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

void SceneState_SetValue14Mode23(void);
s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *object);
s32 SceneActor_CopyActor8PositionWithFixedY(struct Frame *dst);
s32 OverlayObject_IntegrateAndDamp(struct FieldActor *actor);
u8 *ArutinYama_GetExits(void);
void FieldScene_RunSharedSetPiece(s32 a0);
void FieldScene_RunFallingRocksWarning(void);
void ArutinYama_StartAreaFiveEncounter(void);
void SceneActor_UpdateSlot10ByTileX(void);
void ActorPresentation_SetCellAndLowerActorEight(void);
void ArutinYama_RestoreFallingActorNine(void);
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
void ArutinYama_SetActorTenUnderfootPriority(void);
void ArutinYama_SetActorTenAutomaticPriority(void);
void ArutinYama_PushMovableObject(void);
void ArutinYama_RestoreAreaThree(void);
void ArutinYama_UpdateDriftingLeaf(union FieldObject *object);
void ArutinYama_RestoreAreaFour(void);
void ArutinYama_RestoreAreaFive(void);
void ArutinYama_RestoreAreaSix(void);
void ArutinYama_RestoreAreaSeven(void);
void ArutinYama_RestoreAreaTen(void);
void ArutinYama_RestoreAreaEleven(void);
void ArutinYama_InitLeaf(struct FieldActor *leaf);
void SceneActor_SetActor8ModeByCounterBit(void);
void RunEventScript01(void);
void ArutinYama_SpawnLeaf(void);
void ArutinYama_StartLeafGust(s32 x, s32 y, s32 z, s32 facing);
void FieldScene_RunScene3a4SequenceG(void);
void SceneState_ForwardByRuntimeSelector(s32 arg);
void FieldScene_RunActorTenFourStepSequence(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunScene3a4SequenceF(void);
void FieldScene_RunScene3a4SequenceE(void);
void SceneAudio_PlayCue183EverySixtyTicks(void);
void SceneActor_SetActor12ModeByActorZeroHeight(void);
void SceneActor_ClearCollisionFlagAndPlaceMarker(s32 no);
void ArutinYama_SetRollingStateAndPlayCue(u16 *record);

#endif
