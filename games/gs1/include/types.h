#ifndef ALCHEMY_TYPES_H
#define ALCHEMY_TYPES_H

/* The fixed-width spellings every reconstructed translation unit uses.
   Before this header each of them carried its own copy: 7,751 typedef lines
   across 1,224 of 1,249 files, which is the same eight declarations restated
   once per function. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

/* m2c spells a truth value this way and the drafts kept it; every source that
   uses it agreed on the same definition. */
typedef int bool;

#define NULL ((void *)0)

/* Shared runtime names: the address symbols remain the ABI, while callers
   can describe the operation they are performing. */
#define WaitFrames Func_080030f8
#define Audio_PlayCue Func_080f9010
#define Object_SetMode Func_08009080
#define Object_SetCallback Func_08009098
#define Object_Spawn Func_08096c80
#define Object_SetVelocity Func_08096bec
#define Runtime_GetObject Func_08077008
#define Item_GetData Func_08077018
#define Item_FindSlot Func_08077038
#define RotateVectorByMagnitude Func_0800447c
#define NormalizeVector Func_080974d8
#define ScheduleCallbackAfterFrames Func_080041d8
#define Modulo Func_080022fc
#define UiWindow_Clear Func_08015060
#define UiText_DrawMessageAt Func_08015078
#define UiText_DrawAt Func_08015080
#define UiNumber_DrawAt Func_080150b0
#define UiIcon_DrawWithFlags Func_080150c8
#define UiIcon_Draw Func_080152d0
#define UiWindow_Commit Func_08015270
#define UiText_DrawQuantity Func_08015120
#define GameFlag_IsSet Func_080770c0
#define GameFlag_Set Func_080770c8
#define GameFlag_Clear Func_080770d0
#define FixedPoint_Ratio Func_080022ec
#define Object_ResetMotion Func_08009140
#define Object_SetPosition Func_08009150
#define Object_CommitPosition Func_08009158
#define Object_Destroy Func_080090d0
#define Object_SetAction Func_08009088
#define UiWindow_Close Func_08015018
#define Ability_GetData Func_08077080
#define Owner_GetState Func_08077394
#define Resource_FindFreeSlot Func_08004080
#define Runtime_AllocateBlock Func_080048f4
#define Resource_CopyData Func_08003fa4
#define ScheduleCallback Func_08004278
#define Object_InitializeMode Func_08009020
#define Scene_GetRecord Func_0808a080
#define Resource_ResetEntry Func_08003f3c
#define Resource_ClearSlotReferences Func_08003f04
#define Resource_ActivateEntry Func_08003f78
#define Resource_InitializeTable Func_0800403c
#define Runtime_AllocateHeapBlock Func_080048b0
#define Resource_GetBuffer Func_080040d0
#define Runtime_PushSlotEntry Func_08003dec
#define Scheduler_ResetTaskTable Func_080040e8
#define Runtime_BumpAllocate Func_08004938
#define Runtime_BumpAllocateAlternatePool Func_08004970
#define UiPalette_SetColor Func_080150b8
#define Menu_AppendResourceEntry Func_080287a8
#define Object_CheckMovementCollision Func_080091d8
#define Ability_GetMaximum Func_08077240
#define Ability_GetAvailability Func_08077248
#define Shop_GetSelectionState Func_08077020
#define UiText_DrawMessage Func_08015040
#define UiText_DrawCharacter Func_0801e7c0
#define Menu_LoadResourceSlot Func_0802875c
#define GetBattleObjectSlot Func_080b7dd0
#define GetMotionRecord Func_080b7f70
#define ActivateBattleObjectSlot Func_080b7e60
#define Object_GetById Func_08092054
#define ObjectTable_Get Func_0808ba1c
#define BattleUnit_Recalculate Func_08077010
#define UiWork_Finalize Func_08016418
#define Actor_ResetMotionAtAnchor Func_080b8000
#define UiText_ShowMessageAndWait Func_080151c8
#define UiText_ShowMessageAndWaitCore Func_080175a0
#define UiWork_FinalizePending Func_08015140
#define UiWork_FinalizePendingCore Func_08019a54
#define UiWork_Create Func_08015038
#define UiWindow_Create Func_080162d4
#define UiWindow_CreateFar Func_08015010
#define UiWork_IsComplete Func_08017364
#define UiWork_IsCompleteFar Func_08015048
#define UiText_BuildRenderEntries Func_08018038
#define UiText_PrepareMessageWork Func_080174f8
#define AudioTrack_ResetSlotBuckets Func_080f7db4
#define AudioTrack_InsertSlotNode Func_080f7df0
#define AudioTrack_RemoveSlotNode Func_080f7e34
#define AudioTrack_ConsumeSlotBytes Func_080f7e60
#define EmitRandomParticleEffect Func_0808eee4
#define InitializeEventObject Func_0809728c
#define UpdateRisingParticleBurst Func_080981b0
#define SpawnHeavyImpactEffect Func_08098c08
#define CheckObjectMapTile Func_08099678
#define InitializeAnimationObjects Func_0800b7c0
#define InitializeEntryObjects Func_080a1870
#define CreateSideObject Func_0801ec6c
#define UpdateNameEntries Func_080b5e14
#define Inn_CalculateRoomPrice Func_080b3210
#define Inn_CheckIn Func_080b3284
#define Inn_PlaySleepSequence Func_080b3398
#define ConfigureSceneCamera Func_080b8fd4
#define UpdateRadialBurstEffect Func_08098b10
#define UpdateRadialCameraEffect Func_0809aa98
#define UpdateDisplayTransition Func_080903bc
#define MapEvent_RunTileTriggerSequence Func_08099738
#define MusicPlayer_SetVolume Func_080fb2cc
#define MusicPlayer_SetPitch Func_080fb334
#define MusicPlayer_SetPan Func_080fb3a8
#define MusicTrack_ClearModulation Func_080fb410
#define MusicPlayer_SetModulationDepth Func_080fb430
#define MusicPlayer_SetLfoSpeed Func_080fb4a4
#define MusicTrack_DispatchExtendedCommand Func_080fb670
#define MusicTrack_ReadWavePointer Func_080fb6a4
#define MusicTrack_SetToneType Func_080fb6ec
#define MusicTrack_SetToneAttack Func_080fb700
#define MusicTrack_SetToneDecay Func_080fb714
#define MusicTrack_SetToneSustain Func_080fb728
#define MusicTrack_SetToneRelease Func_080fb73c
#define MusicTrack_SetPseudoEchoVolume Func_080fb750
#define MusicTrack_SetPseudoEchoLength Func_080fb75c
#define MusicTrack_SetToneLength Func_080fb768
#define MusicTrack_SetTonePanSweep Func_080fb77c
#define MusicTrack_ExtendedCommandTable Data_080fba48
#define BattleEffect_SpawnFallingParticles Func_08099d18
#define BattleEffect_SpawnBurstParticle Func_08092624
#define BattleEffect_RunRisingObjectSequence Func_08092708
#define BattleEffect_UpdateOrbitAndReturn Func_08097f80
#define BattleEffect_UpdateRadialLaunch Func_0809b11c
#define BattleEffect_UpdateRadialSpread Func_08096048
#define BattleEffect_EmitRandomParticle Func_0808ee0c
#define BattleEffect_FinishHeavyImpact Func_08095a44
#define Object_CollectResources Func_0801c7fc
#define Owner_BuildDigitTiles Func_080798e0
#define UiWork_ActivateChannel Func_08016670
#define UiWork_DrainPending Func_080197c4
#define Menu_SelectResource Func_080286a0
#define Menu_LoadSelectedResource Func_0801c188
#define AffineMatrix_BuildForEffect Func_08003d28
#define Localization_LookupEntryId Func_08019d2c
#define ObjectPlacement_CreateGroup Func_0801fe2c
#define ObjectMotion_MoveTowardTarget Func_0809397c
#define ObjectMotion_PlaceWithinCameraBounds Func_080933f8
#define ObjectEffect_PrepareContextEffect Func_080942e0
#define ObjectGroup_UpdateMembers Func_080d6888
#define Menu_RunActionFlow Func_080a76d0
#define Map_DecodeTileRows Func_0800f9f4
#define DisplayBlend_RunScript Func_080119cc
#define Summon_TakeCharge Func_080c1df4
#define BattleEffect_UpdateRadialMotion Func_080b2f4c
#define ObjectMotion_SetAngleToward Func_0809280c
#define Object_LinkPair Func_08092848
#define ObjectTable_DestroyById Func_08092924
#define Object_SetTargetAndCallback Func_08092a1c
#define ObjectMotion_ArmCallback Func_08092adc
#define ObjectVisual_CopyAttributes Func_08092b54
#define ObjectTable_FindActiveByValue Func_08092be0
#define ObjectGroup_ConfigureChildValue Func_08092950
#define ObjectGroup_ApplyIndexedChildValue Func_08092980
#define ObjectGroup_SetChildValue Func_080929d8
#define ObjectMotion_SetActionVariant Func_08092b08
#define ObjectMotion_SetActionCallback Func_08093a6c
#define ObjectMotion_WaitForAnimationChange Func_08092504
#define ObjectMotion_TurnTowardLinkedTarget Func_08093a14
#define Audio_InitializeRuntimeDefaults Func_080f9438
#define Graphics_ScaleRgb555 Func_080f6038
#define Audio_ClearChannelChain Func_080f9a30
#define MusicTrack_Finish Func_080f9a50
#define Audio_DummyCallback Func_080fb790
#define ObjectLink_RotatePairToward Func_08092878
#define ObjectTable_ReadActiveValue Func_08092ba8
#define ObjectMotion_SetPositionWithTerrain Func_08092454
#define ObjectMotion_SetHorizontalPositionWithTerrain Func_080923e4
#define ObjectMotion_SnapHeadingAndOffset Func_08092208
#define ObjectMotion_Launch Func_08092560
#define ParticleEffect_UpdateLinearMotion Func_080925e0
#define ObjectMotion_StepAngle Func_08092a74
#define MusicPlayer_CopyCommandTable Func_080f9a80
#define Audio_Clear64Bytes Func_080f9a18
#define MusicTrack_ReadCommandByteFiltered Func_080f9ab4
#define Audio_ResumePlayer Func_080fa264
#define MusicTrack_EndTie Func_080fa16c
#define MusicTrack_SetLfoSpeedFromCommand Func_080fa1d4
#define MusicTrack_SetModulationFromCommand Func_080fa1e8
#define MusicTrack_Stop Func_080f9ef8
#define CgbAudio_Update Func_080fae58
#define Math_UmulHigh32 Func_080f95e0
#define AudioEngine_RunMixerTick Func_080f95f0
#define MusicTrack_HandleNote Func_080f9f6c

#endif
