#ifndef ALCHEMY_UNIT_FIELD_KOROSSEO_MARUTA_MARUTA_H
#define ALCHEMY_UNIT_FIELD_KOROSSEO_MARUTA_MARUTA_H

#include "types.h"
#include "field_scene.h"

typedef struct SceneParticle
{
	u8	filler00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	filler14[4];
	s32 scale_x;
	s32 scale_y;
	u8	filler20[0x1c];
	s32 anchor_y;
	u8	filler40[0x15];
	u8	state;
	u8	filler56[0xe];
	s16 velocity_x;
	s16 velocity_y;
} SceneParticle;

typedef struct StageEffect
{
	s32 reserved_00;
	s32 reserved_04;
	s32 x;
	s32 y;
	s32 z;
	u8	reserved_14[20];
	s32 vertical_motion;
	u8	reserved_2c[41];
	u8	state;
} StageEffect;

typedef struct StageEffect_02003d88
{
	s32 reserved_00;
	s32 reserved_04;
	s32 x;
	s32 y;
	s32 z;
	u8	reserved_14[65];
	u8	state;
	u8	reserved_56[14];
	s16 linked_effect_slot;
} StageEffect_02003d88;

u8 *ColossoLogRollingStage_GetScriptData(void);
s32 ColossoLogRollingStage_GetMessageData(void);
u8 *ColossoLogRollingStage_GetActorData(void);
u8 *ColossoLogRollingStage_GetEffectData(void);
void ColossoLogRollingStage_SceneTask(void);
void ColossoLogRollingStage_ResetAndRunSceneTask(void);
void ColossoLogRollingStage_StartSceneTask(void);
void ColossoLogRollingStage_WaitForSceneTask(void);
void ColossoLogRollingStage_NudgeActorsLeft(void);
void ColossoLogRollingStage_ConfigureGridRegion(void);
void ColossoLogRollingStage_ConfigurePrimaryObjects(void);
void FieldScene_RunClosingAuxiliarySequence(void);
void ColossoLogRollingStage_ConfigureSecondaryObjects(void);
void FieldScene_RunFinalAuxiliarySequence(void);
void ColossoLogRollingStage_RunSetupCompletionHooks(void);
void ColossoLogRollingStage_ConfigureActorThirteen(void);
void ColossoLogRollingStage_NoopSetupHook(void);
void ColossoLogRollingStage_RunSetupHook(void);
void ColossoLogRollingStage_ActivateClearObstacleActors(void);
void ColossoLogRollingStage_ShowActorPositionMessage(void);
s32 ColossoLogRollingStage_CheckObstacleDestination(s32 x, s32 z);
s32 ColossoLogRollingStage_CheckPathClearance(s32 x, s32 y);
void FieldScene_RunEarlySequence(void);
s32 ColossoLogRollingStage_SetSceneEventValues(void);
void ColossoLogRollingStage_ConfigureSceneEventEffect(void);
void ColossoLogRollingStage_WaitForSceneEventTask(void);
void ColossoLogRollingStage_OffsetActiveActor(void);
void ColossoLogRollingStage_ClampAndOffsetActiveActor(void);
void ColossoLogRollingStage_NoopSceneEventHook(void);
void KorosseoMaruta_RunCompetitorTalk(s32 a0);
void KorosseoMaruta_RunStageStart(void);
void ColossoLogRollingStage_RunSceneEventIfReady(void);
void ColossoLogRollingStage_FinishOrContinueSceneEvent(void);
s32 ColossoLogRollingStage_GetSceneEventState(void);
s32 StageSetup_BuildAndDispatch(void);
s32 ColossoLogRollingStage_AdvanceParticleMotion(SceneParticle *particle);
void ColossoLogRollingStage_SpawnPeriodicParticle(void);
void FieldScene_RunMultiPhaseActorSequence(s32 a0);
void Korosseo_RunGreetScene(s32 a0);
void FieldScene_RunDualArrivalSequence(s32 scene);
void FieldScene_RunSecondArrivalSequence(s32 scene);
void KorosseoMaruta_RunStageIntro(s32 scene);
void FieldScene_RunFourStepActorMotion(s32 event);
void ColossoLogRollingStage_PositionActor(s32 selector, s32 x, s32 z);
void ColossoLogRollingStage_PositionAndActivateActor(s32 selector, s32 x, s32 z);
void FieldScene_RunOpeningAuxiliarySequence(s32 event);
void ColossoLogRollingStage_RunLogRollingInteraction(s32 actor);
void ColossoLogRollingStage_RestoreActorPositions(void);
void ColossoLogRollingStage_MarkSceneProgress(void);
void ColossoLogRollingStage_SelectNearestObstacle(void);
void ColossoLogRollingStage_ClearSavedActorPositions(void);
s32 ColossoLogRollingStage_RunStateInteraction(s32 actor, s32 flags);
void ColossoLogRollingStage_InitializeStateInteraction(s32 actor, s32 flags);
/* FieldScene_RunMiddleSequence is called before its definition with no prototype in scope. */
void ColossoLogRollingStage_ApplyItemToMatchingSlots(s32 handle, s32 item);
void ColossoLogRollingStage_InitializeModeTask(u32 mode, u32 parameter);
void ColossoLogRollingStage_RunScriptedTransition(s32 mode);
void FieldScene_RunScene3bcSequenceA(s32 a0);
void ColossoLogRollingStage_ResetActorMotion(s32 selector);
void ColossoLogRollingStage_EnsurePaletteHandle(void);
/* ColossoLogRollingStage_StartPaletteTask is called before its definition with no prototype in scope. */
void ColossoLogRollingStage_StartPaletteTaskFromState(u32 first_value, u32 second_value, u32 mode);
void ColossoLogRollingStage_StopPaletteTask(void);
void ColossoLogRollingStage_PositionScaledObject(s32 id, s32 x, s32 z);
/* ColossoLogRollingStage_SpawnPositionedObject is called before its definition with no prototype in scope. */
void WriteU32AsHex(u8 *hex_text, u32 value);
void ColossoLogRollingStage_NoopSceneHook(void);
void ColossoLogRollingStage_SetBalanceStateReady(void);
void ColossoLogRollingStage_WaitForBalanceState(void);
void ColossoLogRollingStage_SpawnRandomSceneEffect(StageEffect *source);
s32 ColossoLogRollingStage_RaiseLinkedSceneEffect(StageEffect_02003d88 *source);
s32 ColossoLogRollingStage_PositionActiveActor(s32 first_handle, s32 second_handle);
/* ColossoLogRollingStage_SetupSceneDescriptor is called before its definition with no prototype in scope. */
void ColossoLogRollingStage_InitializeSceneControl(s32 resource);
void *SceneData_FindSlotAtPosition(void *position, void *self);
void ColossoLogRollingStage_PushStagedActor(void);
void *SceneActor_FindOccupantAheadOfSubject(void);
extern const s32 KorosseoMaruta_Record30Script[];
extern const s8 KorosseoMaruta_LogPattern[][6];
extern const u16 gColossoEarlySequenceData[];
extern const s32 gColossoSceneEventEffect[];
extern const u8 Korosseo_PortraitPaletteOffsets[];
extern const s16 gColossoModeScript2[];
extern const s16 gColossoModeScript3B[];
extern char gColossoHexChars[20];
extern s32 gColossoRandomEffect[];
extern s32 gColossoLinkedEffect[];
extern u32 StagedActor_DirectionSteps[];
extern struct SceneEntrance gKorosseoMarutaEntrances[];
extern u32 gKorosseoMarutaExits[];
extern struct ScenePlacement gKorosseoMarutaPlacements[];
extern s32 gColossoSceneTaskStatus;
extern s32 gColossoSceneTaskState;
extern struct SceneEvent gKorosseoMarutaEvents[];
extern s32 KorosseoMaruta_Actor8Action[];
extern s32 KorosseoMaruta_LeaderActionA[];
extern s32 KorosseoMaruta_LeaderActionB[];
extern s32 KorosseoMaruta_LeaderActionC[];
extern s32 gColossoMultiPhaseData[];
extern s32 gColossoParticleKinds[];
extern s16 Korosseo_PortraitSlot;
extern s16 gColossoModeScriptDefault[];
extern s16 gColossoModeScript4[];
extern s16 gColossoModeScript3A[];
extern s16 Korosseo_MarkerSlot;
extern s32 Korosseo_RivalFinishScript[];
extern s32 KorosseoMaruta_PlaceScript[];

#endif
