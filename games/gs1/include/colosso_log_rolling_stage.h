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

#define Colosso_GetScriptData Func_02000030
#define Colosso_GetMessageData Func_02000038
#define Colosso_GetActorData Func_0200003c
#define Colosso_GetEffectData Func_02000044
#define Colosso_ResetAndRunSceneTask Func_02000188
#define Colosso_StartSceneTask Func_020001b4
#define Colosso_WaitForSceneTask Func_020001c8
#define Colosso_NudgeActorsLeft Func_02000204
#define Colosso_ConfigureGridRegion Func_0200024c
#define Colosso_ConfigurePrimaryObjects Func_02000274
#define Colosso_ConfigureSecondaryObjects Func_02000404
#define Colosso_RunSetupCompletionHooks Func_0200057c
#define Colosso_ConfigureActorThirteen Func_0200058c
#define Colosso_NoopSetupHook Func_020005d0
#define Colosso_RunSetupHook Func_020005d4
#define Colosso_ActivateClearObstacleActors Func_020005e0
#define Colosso_ShowActorPositionMessage Func_02000658
#define Colosso_CheckObstacleDestination Func_020006a4
#define Colosso_CheckPathClearance Func_02000714
#define Colosso_SetSceneEventValues Func_02000a20
#define Colosso_ConfigureSceneEventEffect Func_02000a44
#define Colosso_WaitForSceneEventTask Func_02000a84
#define Colosso_OffsetActiveActor Func_02000ad4
#define Colosso_ClampAndOffsetActiveActor Func_02000b30
#define Colosso_NoopSceneEventHook Func_02000ba0
#define Colosso_RunSceneEventIfReady Func_02000d70
#define Colosso_FinishOrContinueSceneEvent Func_02000d84
#define Colosso_GetSceneEventState Func_02000d9c
#define Colosso_AdvanceParticleMotion Func_0200137c
#define Colosso_SpawnPeriodicParticle Func_020013c0
#define Colosso_PositionActor Func_020022c4
#define Colosso_PositionAndActivateActor Func_020022f4
#define Colosso_RunLogRollingInteraction Func_020024d0
#define Colosso_RestoreActorPositions Func_020025c8
#define Colosso_MarkSceneProgress Func_02002710
#define Colosso_SelectNearestObstacle Func_02002758
#define Colosso_ClearSavedActorPositions Func_02002a50
#define Colosso_RunStateInteraction Func_02002a94
#define Colosso_InitializeStateInteraction Func_02002b50
#define Colosso_ApplyItemToMatchingSlots Func_02002e18
#define Colosso_InitializeModeTask Func_020033d8
#define Colosso_RunScriptedTransition Func_02003468
#define Colosso_ResetActorMotion Func_020038dc
#define Colosso_EnsurePaletteHandle Func_020038fc
#define Colosso_StartPaletteTask Func_02003a58
#define Colosso_StartPaletteTaskFromState Func_02003abc
#define Colosso_StopPaletteTask Func_02003b18
#define Colosso_PositionScaledObject Func_02003b40
#define Colosso_SpawnPositionedObject Func_02003b80
#define Colosso_NoopSceneHook Func_02003cc0
#define Colosso_SetBalanceStateReady Func_02003cc4
#define Colosso_WaitForBalanceState Func_02003cd0
#define Colosso_SpawnRandomSceneEffect Func_02003cf4
#define Colosso_RaiseLinkedSceneEffect Func_02003d88
#define Colosso_PositionActiveActor Func_02003ddc
#define Colosso_SetupSceneDescriptor Func_02004494
#define Colosso_InitializeSceneControl Func_0200457c
#define Colosso_SetSceneControlValue Func_020045d0
#define Colosso_PushStagedActor Func_02004628
#define Colosso_FindActorAhead Func_02004790

#endif
