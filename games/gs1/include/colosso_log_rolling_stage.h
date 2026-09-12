#ifndef COLOSSO_LOG_ROLLING_STAGE_H
#define COLOSSO_LOG_ROLLING_STAGE_H

#include "types.h"

u8 *ColossoLogRollingStage_GetScriptData(void);
s32 ColossoLogRollingStage_GetMessageData(void);
u8 *ColossoLogRollingStage_GetActorData(void);
u8 *ColossoLogRollingStage_GetEffectData(void);
void ColossoLogRollingStage_ResetAndRunSceneTask(void);
void ColossoLogRollingStage_StartSceneTask(void);
void ColossoLogRollingStage_WaitForSceneTask(void);
void ColossoLogRollingStage_NudgeActorsLeft(void);
void ColossoLogRollingStage_ConfigureGridRegion(void);
void ColossoLogRollingStage_ConfigurePrimaryObjects(void);
void ColossoLogRollingStage_ConfigureSecondaryObjects(void);
void ColossoLogRollingStage_RunSetupCompletionHooks(void);
void ColossoLogRollingStage_ConfigureActorThirteen(void);
void ColossoLogRollingStage_NoopSetupHook(void);
void ColossoLogRollingStage_RunSetupHook(void);
void ColossoLogRollingStage_ActivateClearObstacleActors(void);
void ColossoLogRollingStage_ShowActorPositionMessage(void);
s32 ColossoLogRollingStage_CheckObstacleDestination(s32 x, s32 z);
s32 ColossoLogRollingStage_CheckPathClearance(s32 x, s32 y);
s32 ColossoLogRollingStage_SetSceneEventValues(void);
void ColossoLogRollingStage_ConfigureSceneEventEffect(void);
void ColossoLogRollingStage_WaitForSceneEventTask(void);
void ColossoLogRollingStage_OffsetActiveActor(void);
void ColossoLogRollingStage_ClampAndOffsetActiveActor(void);
void ColossoLogRollingStage_NoopSceneEventHook(void);
void ColossoLogRollingStage_RunSceneEventIfReady(void);
void ColossoLogRollingStage_FinishOrContinueSceneEvent(void);
s32 ColossoLogRollingStage_GetSceneEventState(void);
s32 ColossoLogRollingStage_AdvanceParticleMotion();
void ColossoLogRollingStage_SpawnPeriodicParticle(void);
void ColossoLogRollingStage_PositionActor(s32 selector, s32 x, s32 z);
void ColossoLogRollingStage_PositionAndActivateActor(s32 selector, s32 x, s32 z);
void ColossoLogRollingStage_RunLogRollingInteraction(s32 actor);
void ColossoLogRollingStage_RestoreActorPositions(void);
void ColossoLogRollingStage_MarkSceneProgress(void);
void ColossoLogRollingStage_SelectNearestObstacle(void);
void ColossoLogRollingStage_ClearSavedActorPositions(void);
s32 ColossoLogRollingStage_RunStateInteraction(s32 actor_handle, s32 interaction_base);
void ColossoLogRollingStage_InitializeStateInteraction(s32 actor_handle, s32 interaction_base);
void ColossoLogRollingStage_ApplyItemToMatchingSlots(s32 handle, s32 item);
void ColossoLogRollingStage_InitializeModeTask(u32 mode, u32 parameter);
void ColossoLogRollingStage_RunScriptedTransition(s32 mode);
void ColossoLogRollingStage_ResetActorMotion(s32 selector);
void ColossoLogRollingStage_EnsurePaletteHandle(void);
void ColossoLogRollingStage_StartPaletteTask(u32 first_value, u32 second_value, u32 mode);
void ColossoLogRollingStage_StartPaletteTaskFromState(u32 first_value, u32 second_value, u32 mode);
void ColossoLogRollingStage_StopPaletteTask(void);
void ColossoLogRollingStage_PositionScaledObject(s32 id, s32 x, s32 z);
void ColossoLogRollingStage_SpawnPositionedObject(s32 object_id, s32 x, s32 z);
void ColossoLogRollingStage_NoopSceneHook(void);
void ColossoLogRollingStage_SetBalanceStateReady(void);
void ColossoLogRollingStage_WaitForBalanceState(void);
void ColossoLogRollingStage_SpawnRandomSceneEffect();
s32 ColossoLogRollingStage_RaiseLinkedSceneEffect();
s32 ColossoLogRollingStage_PositionActiveActor(s32 first_handle, s32 second_handle);
void ColossoLogRollingStage_SetupSceneDescriptor(s32 first_actor, s32 second_actor, s32 mode, s32 centre, s32 extra, s32 third_actor, s32 fourth_actor);
void ColossoLogRollingStage_InitializeSceneControl(void);
void ColossoLogRollingStage_SetSceneControlValue(u16 value);
void ColossoLogRollingStage_PushStagedActor(void);
s32 *ColossoLogRollingStage_FindActorAhead(void);

#endif
