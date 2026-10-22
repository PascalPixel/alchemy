.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Func_02003a14
	.thumb_func
Func_02003a14:
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
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02003a54
	.thumb_func
Func_02003a54:
	overlay_veneer Object_SetMode
	.global Func_02003a5c
	.thumb_func
Func_02003a5c:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Func_080200c0
	.global Func_02003a6c
	.thumb_func
Func_02003a6c:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02003a7c
	.thumb_func
Func_02003a7c:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02003a84
	.thumb_func
Func_02003a84:
	overlay_veneer Object_SetPosition
	.global Func_02003a8c
	.thumb_func
Func_02003a8c:
	overlay_veneer Object_CommitPosition
	.global Func_02003a94
	.thumb_func
Func_02003a94:
	overlay_veneer Map_CopyMetatileIndicesRectFar
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
	.global Func_02003aa4
	.thumb_func
Func_02003aa4:
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
	.global Func_02003aac
	.thumb_func
Func_02003aac:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003ac4
	.thumb_func
Func_02003ac4:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02003acc
	.thumb_func
Func_02003acc:
	overlay_veneer Func_08020230
	.global Func_02003ad4
	.thumb_func
Func_02003ad4:
	overlay_veneer SetMapCellCollisionFar
	.global Func_02003adc
	.thumb_func
Func_02003adc:
	overlay_veneer Func_080202f0
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02003aec
	.thumb_func
Func_02003aec:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02003af4
	.thumb_func
Func_02003af4:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global Func_02003b0c
	.thumb_func
Func_02003b0c:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02003b14
	.thumb_func
Func_02003b14:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetActionById
	.thumb_func
Object_SetActionById:
	overlay_veneer Object_SetActionByIdFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02003b3c
	.thumb_func
Func_02003b3c:
	overlay_veneer Func_080c8208
	.global Func_02003b44
	.thumb_func
Func_02003b44:
	overlay_veneer Func_080c8218
	.global ObjectVisual_CopyAttributes
	.thumb_func
ObjectVisual_CopyAttributes:
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02003b5c
	.thumb_func
Func_02003b5c:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003b6c
	.thumb_func
Func_02003b6c:
	overlay_veneer Func_080c8240
	.global Func_02003b74
	.thumb_func
Func_02003b74:
	overlay_veneer Func_080c8250
	.global Func_02003b7c
	.thumb_func
Func_02003b7c:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02003b84
	.thumb_func
Func_02003b84:
	overlay_veneer Func_080c8288
	.global Func_02003b8c
	.thumb_func
Func_02003b8c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c82d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c82d0
	.else
	overlay_veneer Func_080c82e0
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
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8488
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8488
	.else
	overlay_veneer Field_DispatchTypeHandlerFar
	.endif
	.endif
	.global Func_02003bac
	.thumb_func
Func_02003bac:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8490
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8490
	.else
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.endif
	.endif
	.global Func_02003bb4
	.thumb_func
Func_02003bb4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	overlay_veneer BattleEffect_SetupObjectPairFar
	.endif
	.endif
	.global Func_02003bbc
	.thumb_func
Func_02003bbc:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.else
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.endif
	.endif
	.global Func_02003bc4
	.thumb_func
Func_02003bc4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetupObjectPairFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetupObjectPairFar
	.else
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.endif
	.endif
	.global Func_02003bcc
	.thumb_func
Func_02003bcc:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02003bd4
	.thumb_func
Func_02003bd4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_HasMatchingEvent5Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_HasMatchingEvent5Far
	.else
	overlay_veneer Func_080c8518
	.endif
	.endif
	.global Func_02003bdc
	.thumb_func
Func_02003bdc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_SetActionCallbackFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_SetActionCallbackFar
	.else
	overlay_veneer Event_NoOpFar
	.endif
	.endif
	.global Func_02003be4
	.thumb_func
Func_02003be4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8698
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8698
	.else
	overlay_veneer Func_080c86a8
	.endif
	.endif
	.global Func_02003bec
	.thumb_func
Func_02003bec:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c0
	.else
	overlay_veneer Func_080c86d0
	.endif
	.endif
	.global Func_02003bf4
	.thumb_func
Func_02003bf4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c8
	.else
	overlay_veneer Func_080c86d8
	.endif
	.endif
	.global Func_02003bfc
	.thumb_func
Func_02003bfc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86d8
	.else
	overlay_veneer Func_080c86e8
	.endif
	.endif
	.global Func_02003c04
	.thumb_func
Func_02003c04:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_02003c0c
	.thumb_func
Func_02003c0c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8708
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8708
	.else
	overlay_veneer Func_080c8718
	.endif
	.endif
	.global Func_02003c14
	.thumb_func
Func_02003c14:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8710
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8710
	.else
	overlay_veneer Func_080c8720
	.endif
	.endif
	.global Func_02003c1c
	.thumb_func
Func_02003c1c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8718
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8718
	.else
	overlay_veneer Func_080c8728
	.endif
	.endif
	.global Func_02003c24
	.thumb_func
Func_02003c24:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8720
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8720
	.else
	overlay_veneer Func_080c8730
	.endif
	.endif
	.global Func_02003c2c
	.thumb_func
Func_02003c2c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8728
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8728
	.else
	overlay_veneer Func_080c8738
	.endif
	.endif
	.global Func_02003c34
	.thumb_func
Func_02003c34:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b0
	.else
	overlay_veneer Func_080c87c0
	.endif
	.endif
	.global Func_02003c3c
	.thumb_func
Func_02003c3c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87c0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87c0
	.else
	overlay_veneer Func_080c87d0
	.endif
	.endif
	.global Func_02003c44
	.thumb_func
Func_02003c44:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87d0
	.else
	overlay_veneer BattleEffect_StepVector3Far
	.endif
	.endif
	.global Func_02003c4c
	.thumb_func
Func_02003c4c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88b8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88b8
	.else
	overlay_veneer Func_080c88c8
	.endif
	.endif
	.global Func_02003c54
	.thumb_func
Func_02003c54:
	overlay_veneer Audio_PlayCue
