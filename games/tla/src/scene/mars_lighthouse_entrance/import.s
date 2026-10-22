.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer ArcTan2Far
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global MemAlloc
	.thumb_func
MemAlloc:
	overlay_veneer Runtime_BumpAllocateFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02004b80
	.thumb_func
Func_02004b80:
	overlay_veneer Runtime_PushSlotEntryFar
	.global UnitGet
	.thumb_func
UnitGet:
	overlay_veneer UnitGetFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global ResourceMetadata_ClearRecord
	.thumb_func
ResourceMetadata_ClearRecord:
	overlay_veneer ResourceMetadata_ClearRecordFar
	.global Func_02004bc0
	.thumb_func
Func_02004bc0:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_02004bd0
	.thumb_func
Func_02004bd0:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02004bd8
	.thumb_func
Func_02004bd8:
	overlay_veneer Func_080200c0
	.global Func_02004be0
	.thumb_func
Func_02004be0:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02004bf0
	.thumb_func
Func_02004bf0:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02004bf8
	.thumb_func
Func_02004bf8:
	overlay_veneer Object_SetPosition
	.global Func_02004c00
	.thumb_func
Func_02004c00:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020190
	.else
	overlay_veneer Map_GetTerrainHeightFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02004c10
	.thumb_func
Func_02004c10:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020198
	.else
	overlay_veneer GetMapCellCollisionFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02004c18
	.thumb_func
Func_02004c18:
	.ifdef TLA_EDITION_DE
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.else
	overlay_veneer Func_080201d8
	.endif
	.endif
	.endif
	.endif
	.global Func_02004c20
	.thumb_func
Func_02004c20:
	.ifdef TLA_EDITION_DE
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	overlay_veneer Map_CopyCellsRectFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02004c28
	.thumb_func
Func_02004c28:
	.ifdef TLA_EDITION_DE
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	overlay_veneer Map_CopyCellAttributeRectFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02004c30
	.thumb_func
Func_02004c30:
	.ifdef TLA_EDITION_DE
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Map_GetTerrainHeightFar
	.else
	overlay_veneer Func_080201f0
	.endif
	.endif
	.endif
	.endif
	.global Func_02004c38
	.thumb_func
Func_02004c38:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004c50
	.thumb_func
Func_02004c50:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02004c58
	.thumb_func
Func_02004c58:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02004c68
	.thumb_func
Func_02004c68:
	overlay_veneer ObjectDispatch_SetSpritePriorityFar
	.global Func_02004c70
	.thumb_func
Func_02004c70:
	overlay_veneer Func_080202f8
	.global Func_02004c78
	.thumb_func
Func_02004c78:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020318
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020318
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020318
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020318
	.else
	overlay_veneer Runtime_CheckRadiusOverlapFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02004c80
	.thumb_func
Func_02004c80:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020330
	.else
	overlay_veneer Func_08020360
	.endif
	.endif
	.endif
	.endif
	.global Func_02004c88
	.thumb_func
Func_02004c88:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020388
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020388
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020388
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020388
	.else
	overlay_veneer Func_08020390
	.endif
	.endif
	.endif
	.endif
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02004ca8
	.thumb_func
Func_02004ca8:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02004cb0
	.thumb_func
Func_02004cb0:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global Func_02004cd8
	.thumb_func
Func_02004cd8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02004ce8
	.thumb_func
Func_02004ce8:
	overlay_veneer BattleEffect_RunRisingObjectSequenceFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02004d00
	.thumb_func
Func_02004d00:
	overlay_veneer Func_080c8210
	.global Func_02004d08
	.thumb_func
Func_02004d08:
	overlay_veneer Func_080c8218
	.global Func_02004d10
	.thumb_func
Func_02004d10:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02004d20
	.thumb_func
Func_02004d20:
	overlay_veneer Func_080c8240
	.global Func_02004d28
	.thumb_func
Func_02004d28:
	overlay_veneer Func_080c8258
	.global Func_02004d30
	.thumb_func
Func_02004d30:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02004d38
	.thumb_func
Func_02004d38:
	overlay_veneer Func_080c8288
	.global Func_02004d40
	.thumb_func
Func_02004d40:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c82d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c82d0
	.else
	overlay_veneer Func_080c82e0
	.endif
	.endif
	.global Func_02004d48
	.thumb_func
Func_02004d48:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c82e8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c82e8
	.else
	overlay_veneer EventRuntime_PrepareCurrentObjectFar
	.endif
	.endif
	.global Func_02004d50
	.thumb_func
Func_02004d50:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8340
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8340
	.else
	overlay_veneer DisplayTransition_StartFar
	.endif
	.endif
	.global Func_02004d58
	.thumb_func
Func_02004d58:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8368
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8368
	.else
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.endif
	.endif
	.global Func_02004d60
	.thumb_func
Func_02004d60:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.else
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.endif
	.endif
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	overlay_veneer Event_StartScreenTransitionFar
	.endif
	.endif
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetPrimaryBufferValueFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetPrimaryBufferValueFar
	.else
	overlay_veneer Event_FinishScreenTransitionFar
	.endif
	.endif
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_DelayEffectFramesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_DelayEffectFramesFar
	.else
	overlay_veneer Event_WaitTransitionFramesFar
	.endif
	.endif
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_SpawnObjectTableFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_SpawnObjectTableFar
	.else
	overlay_veneer ObjectTable_GetFar
	.endif
	.endif
	.global Func_02004d88
	.thumb_func
Func_02004d88:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global BattleEffect_PrepareBufferInterpolation
	.thumb_func
BattleEffect_PrepareBufferInterpolation:
	.ifdef TLA_EDITION_ES
	overlay_veneer EventObject_InitializeFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer EventObject_InitializeFar
	.else
	overlay_veneer BattleEffect_PrepareBufferInterpolationFar
	.endif
	.endif
	.global Func_02004d98
	.thumb_func
Func_02004d98:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_InitializeSharedSceneFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_InitializeSharedSceneFar
	.else
	overlay_veneer EventRuntime_ResolveAllPendingActionsFar
	.endif
	.endif
	.global Func_02004da0
	.thumb_func
Func_02004da0:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02004da8
	.thumb_func
Func_02004da8:
	.ifdef TLA_EDITION_ES
	overlay_veneer UpdateRisingParticleBurstFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer UpdateRisingParticleBurstFar
	.else
	overlay_veneer BattleParty_ApplyHealthDeltaFar
	.endif
	.endif
	.global Func_02004db0
	.thumb_func
Func_02004db0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_BuildRecordSnapshotFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_BuildRecordSnapshotFar
	.else
	overlay_veneer ObjectTable_SnapshotFar
	.endif
	.endif
	.global Func_02004db8
	.thumb_func
Func_02004db8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_02004dc0
	.thumb_func
Func_02004dc0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.endif
	.endif
	.global Func_02004dc8
	.thumb_func
Func_02004dc8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88c8
	.else
	overlay_veneer Func_080c88d8
	.endif
	.endif
	.global Func_02004dd0
	.thumb_func
Func_02004dd0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88d8
	.else
	overlay_veneer Func_080c88e8
	.endif
	.endif
	.global Func_02004dd8
	.thumb_func
Func_02004dd8:
	.ifdef TLA_EDITION_ES
	overlay_veneer ObjectEffect_BeginContextEffect25Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer ObjectEffect_BeginContextEffect25Far
	.else
	overlay_veneer Func_080c8918
	.endif
	.endif
	.global Func_02004de0
	.thumb_func
Func_02004de0:
	overlay_veneer Audio_PlayCue
