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
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global MemAlloc
	.thumb_func
MemAlloc:
	overlay_veneer Runtime_BumpAllocateFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_020029a0
	.thumb_func
Func_020029a0:
	overlay_veneer Resource_DecodeType01Far
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
	.global Func_020029c0
	.thumb_func
Func_020029c0:
	overlay_veneer Runtime_PushSlotEntryFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_020029f0
	.thumb_func
Func_020029f0:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_02002a00
	.thumb_func
Func_02002a00:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02002a08
	.thumb_func
Func_02002a08:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02002a18
	.thumb_func
Func_02002a18:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02002a20
	.thumb_func
Func_02002a20:
	overlay_veneer Object_ResetMotion
	.global Func_02002a28
	.thumb_func
Func_02002a28:
	overlay_veneer Object_SetPosition
	.global Func_02002a30
	.thumb_func
Func_02002a30:
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
	.global Func_02002a40
	.thumb_func
Func_02002a40:
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
	.global Func_02002a48
	.thumb_func
Func_02002a48:
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
	.global Func_02002a50
	.thumb_func
Func_02002a50:
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
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02002a60
	.thumb_func
Func_02002a60:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02002a68
	.thumb_func
Func_02002a68:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02002a78
	.thumb_func
Func_02002a78:
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
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02002a88
	.thumb_func
Func_02002a88:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02002a90
	.thumb_func
Func_02002a90:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global Func_02002ab8
	.thumb_func
Func_02002ab8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02002ad8
	.thumb_func
Func_02002ad8:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002af0
	.thumb_func
Func_02002af0:
	overlay_veneer Func_080c8240
	.global Func_02002af8
	.thumb_func
Func_02002af8:
	overlay_veneer Func_080c8258
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
	.global Func_02002b18
	.thumb_func
Func_02002b18:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02002b20
	.thumb_func
Func_02002b20:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_02002b28
	.thumb_func
Func_02002b28:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8770
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8770
	.else
	overlay_veneer Func_080c8780
	.endif
	.endif
	.global Func_02002b30
	.thumb_func
Func_02002b30:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b0
	.else
	overlay_veneer Func_080c87c0
	.endif
	.endif
	.global Func_02002b38
	.thumb_func
Func_02002b38:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b8
	.else
	overlay_veneer Func_080c87c8
	.endif
	.endif
	.global Func_02002b40
	.thumb_func
Func_02002b40:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87c0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87c0
	.else
	overlay_veneer Func_080c87d0
	.endif
	.endif
	.global Func_02002b48
	.thumb_func
Func_02002b48:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_StepVector2Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_StepVector2Far
	.else
	overlay_veneer Func_080c87e8
	.endif
	.endif
	.global Func_02002b50
	.thumb_func
Func_02002b50:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetObjectTickModeFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetObjectTickModeFar
	.else
	overlay_veneer Func_080c8830
	.endif
	.endif
	.global Func_02002b58
	.thumb_func
Func_02002b58:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8838
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8838
	.else
	overlay_veneer Func_080c8848
	.endif
	.endif
	.global AudioCommand_WaitForStateByteClear
	.thumb_func
AudioCommand_WaitForStateByteClear:
	overlay_veneer AudioCommand_WaitForStateByteClearFar
	.global Func_02002b68
	.thumb_func
Func_02002b68:
	overlay_veneer Audio_PlayCue
