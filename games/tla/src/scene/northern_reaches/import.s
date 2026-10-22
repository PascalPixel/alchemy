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
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
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
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_020009f0
	.thumb_func
Func_020009f0:
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
	.global Func_02000a10
	.thumb_func
Func_02000a10:
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
	.global Func_02000a30
	.thumb_func
Func_02000a30:
	overlay_veneer Object_CommitPosition
	.global Func_02000a38
	.thumb_func
Func_02000a38:
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
	.global Func_02000a40
	.thumb_func
Func_02000a40:
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
	.global Func_02000a50
	.thumb_func
Func_02000a50:
	overlay_veneer Map_SetScreenShakeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02000a68
	.thumb_func
Func_02000a68:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02000a70
	.thumb_func
Func_02000a70:
	overlay_veneer EventRuntime_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02000ab0
	.thumb_func
Func_02000ab0:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02000ac0
	.thumb_func
Func_02000ac0:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02000ad0
	.thumb_func
Func_02000ad0:
	overlay_veneer Func_080c8198
	.global Func_02000ad8
	.thumb_func
Func_02000ad8:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02000af0
	.thumb_func
Func_02000af0:
	overlay_veneer Func_080c8240
	.global Func_02000af8
	.thumb_func
Func_02000af8:
	overlay_veneer Event_SetDestinationFar
	.global Func_02000b00
	.thumb_func
Func_02000b00:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02000b08
	.thumb_func
Func_02000b08:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8320
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8320
	.else
	overlay_veneer Func_080c8330
	.endif
	.endif
	.global Func_02000b10
	.thumb_func
Func_02000b10:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8368
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8368
	.else
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.endif
	.endif
	.global Func_02000b18
	.thumb_func
Func_02000b18:
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
	.global Func_02000b30
	.thumb_func
Func_02000b30:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global Func_02000b40
	.thumb_func
Func_02000b40:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b0
	.else
	overlay_veneer Func_080c87c0
	.endif
	.endif
	.global Func_02000b48
	.thumb_func
Func_02000b48:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87c0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87c0
	.else
	overlay_veneer Func_080c87d0
	.endif
	.endif
	.global Func_02000b50
	.thumb_func
Func_02000b50:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87d0
	.else
	overlay_veneer BattleEffect_StepVector3Far
	.endif
	.endif
	.global Func_02000b58
	.thumb_func
Func_02000b58:
	overlay_veneer Audio_PlayCue
