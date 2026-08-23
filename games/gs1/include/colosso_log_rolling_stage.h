#ifndef COLOSSO_LOG_ROLLING_STAGE_H
#define COLOSSO_LOG_ROLLING_STAGE_H

#include "types.h"

u8 *Func_02000030(void);
s32 Func_02000038(void);
u8 *Func_0200003c(void);
u8 *Func_02000044(void);
void Func_02000188(void);
void Func_020001b4(void);
void Func_020001c8(void);
void Func_02000204(void);
void Func_0200024c(void);
void Func_02000274(void);
void Func_02000404(void);
void Func_0200057c(void);
void Func_0200058c(void);
void Func_020005d0(void);
void Func_020005d4(void);
void Func_020005e0(void);
void Func_02000658(void);
s32 Func_020006a4(s32 x, s32 z);
s32 Func_02000714(s32 x, s32 y);
s32 Func_02000a20(void);
void Func_02000a44(void);
void Func_02000a84(void);
void Func_02000ad4(void);
void Func_02000b30(void);
void Func_02000ba0(void);
void Func_02000d70(void);
void Func_02000d84(void);
s32 Func_02000d9c(void);
s32 Func_0200137c();
void Func_020013c0(void);
void Func_020022c4(s32 selector, s32 x, s32 z);
void Func_020022f4(s32 selector, s32 x, s32 z);
void Func_020024d0(s32 actor);
void Func_020025c8(void);
void Func_02002710(void);
void Func_02002758(void);
void Func_02002a50(void);
s32 Func_02002a94(s32 actor_handle, s32 interaction_base);
void Func_02002b50(s32 actor_handle, s32 interaction_base);
void Func_02002e18(s32 handle, s32 item);
void Func_020033d8(u32 mode, u32 parameter);
void Func_02003468(s32 mode);
void Func_020038dc(s32 selector);
void Func_020038fc(void);
void Func_02003a58(u32 first_value, u32 second_value, u32 mode);
void Func_02003abc(u32 first_value, u32 second_value, u32 mode);
void Func_02003b18(void);
void Func_02003b40(s32 id, s32 x, s32 z);
void Func_02003b80(s32 object_id, s32 x, s32 z);
void Func_02003cc0(void);
void Func_02003cc4(void);
void Func_02003cd0(void);
void Func_02003cf4();
s32 Func_02003d88();
s32 Func_02003ddc(s32 first_handle, s32 second_handle);
void Func_02004494(s32 first_actor, s32 second_actor, s32 mode, s32 centre,
                   s32 extra, s32 third_actor, s32 fourth_actor);
void Func_0200457c(void);
void Func_020045d0(u16 value);
void Func_02004628(void);
s32 *Func_02004790(void);

#define ColossoLogRollingStage_GetScriptData Func_02000030
#define ColossoLogRollingStage_GetMessageData Func_02000038
#define ColossoLogRollingStage_GetActorData Func_0200003c
#define ColossoLogRollingStage_GetEffectData Func_02000044
#define ColossoLogRollingStage_ResetAndRunSceneTask Func_02000188
#define ColossoLogRollingStage_StartSceneTask Func_020001b4
#define ColossoLogRollingStage_WaitForSceneTask Func_020001c8
#define ColossoLogRollingStage_NudgeActorsLeft Func_02000204
#define ColossoLogRollingStage_ConfigureGridRegion Func_0200024c
#define ColossoLogRollingStage_ConfigurePrimaryObjects Func_02000274
#define ColossoLogRollingStage_ConfigureSecondaryObjects Func_02000404
#define ColossoLogRollingStage_RunSetupCompletionHooks Func_0200057c
#define ColossoLogRollingStage_ConfigureActorThirteen Func_0200058c
#define ColossoLogRollingStage_NoopSetupHook Func_020005d0
#define ColossoLogRollingStage_RunSetupHook Func_020005d4
#define ColossoLogRollingStage_ActivateClearObstacleActors Func_020005e0
#define ColossoLogRollingStage_ShowActorPositionMessage Func_02000658
#define ColossoLogRollingStage_CheckObstacleDestination Func_020006a4
#define ColossoLogRollingStage_CheckPathClearance Func_02000714
#define ColossoLogRollingStage_SetSceneEventValues Func_02000a20
#define ColossoLogRollingStage_ConfigureSceneEventEffect Func_02000a44
#define ColossoLogRollingStage_WaitForSceneEventTask Func_02000a84
#define ColossoLogRollingStage_OffsetActiveActor Func_02000ad4
#define ColossoLogRollingStage_ClampAndOffsetActiveActor Func_02000b30
#define ColossoLogRollingStage_NoopSceneEventHook Func_02000ba0
#define ColossoLogRollingStage_RunSceneEventIfReady Func_02000d70
#define ColossoLogRollingStage_FinishOrContinueSceneEvent Func_02000d84
#define ColossoLogRollingStage_GetSceneEventState Func_02000d9c
#define ColossoLogRollingStage_AdvanceParticleMotion Func_0200137c
#define ColossoLogRollingStage_SpawnPeriodicParticle Func_020013c0
#define ColossoLogRollingStage_PositionActor Func_020022c4
#define ColossoLogRollingStage_PositionAndActivateActor Func_020022f4
#define ColossoLogRollingStage_RunLogRollingInteraction Func_020024d0
#define ColossoLogRollingStage_RestoreActorPositions Func_020025c8
#define ColossoLogRollingStage_MarkSceneProgress Func_02002710
#define ColossoLogRollingStage_SelectNearestObstacle Func_02002758
#define ColossoLogRollingStage_ClearSavedActorPositions Func_02002a50
#define ColossoLogRollingStage_RunStateInteraction Func_02002a94
#define ColossoLogRollingStage_InitializeStateInteraction Func_02002b50
#define ColossoLogRollingStage_ApplyItemToMatchingSlots Func_02002e18
#define ColossoLogRollingStage_InitializeModeTask Func_020033d8
#define ColossoLogRollingStage_RunScriptedTransition Func_02003468
#define ColossoLogRollingStage_ResetActorMotion Func_020038dc
#define ColossoLogRollingStage_EnsurePaletteHandle Func_020038fc
#define ColossoLogRollingStage_StartPaletteTask Func_02003a58
#define ColossoLogRollingStage_StartPaletteTaskFromState Func_02003abc
#define ColossoLogRollingStage_StopPaletteTask Func_02003b18
#define ColossoLogRollingStage_PositionScaledObject Func_02003b40
#define ColossoLogRollingStage_SpawnPositionedObject Func_02003b80
#define ColossoLogRollingStage_NoopSceneHook Func_02003cc0
#define ColossoLogRollingStage_SetBalanceStateReady Func_02003cc4
#define ColossoLogRollingStage_WaitForBalanceState Func_02003cd0
#define ColossoLogRollingStage_SpawnRandomSceneEffect Func_02003cf4
#define ColossoLogRollingStage_RaiseLinkedSceneEffect Func_02003d88
#define ColossoLogRollingStage_PositionActiveActor Func_02003ddc
#define ColossoLogRollingStage_SetupSceneDescriptor Func_02004494
#define ColossoLogRollingStage_InitializeSceneControl Func_0200457c
#define ColossoLogRollingStage_SetSceneControlValue Func_020045d0
#define ColossoLogRollingStage_PushStagedActor Func_02004628
#define ColossoLogRollingStage_FindActorAhead Func_02004790

#endif
