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
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
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
	.global Func_02003ba0
	.thumb_func
Func_02003ba0:
	overlay_veneer Object_SetMode
	.global Func_02003ba8
	.thumb_func
Func_02003ba8:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02003bb0
	.thumb_func
Func_02003bb0:
	overlay_veneer Func_080200c0
	.global Func_02003bb8
	.thumb_func
Func_02003bb8:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02003bc8
	.thumb_func
Func_02003bc8:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02003bd0
	.thumb_func
Func_02003bd0:
	overlay_veneer Object_SetPosition
	.global Func_02003bd8
	.thumb_func
Func_02003bd8:
	overlay_veneer Object_CommitPosition
	.global Func_02003be0
	.thumb_func
Func_02003be0:
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
	.global Func_02003bf0
	.thumb_func
Func_02003bf0:
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
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02003c00
	.thumb_func
Func_02003c00:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02003c08
	.thumb_func
Func_02003c08:
	overlay_veneer Func_08020230
	.global Func_02003c10
	.thumb_func
Func_02003c10:
	overlay_veneer Func_080202f0
	.global Func_02003c18
	.thumb_func
Func_02003c18:
	overlay_veneer Func_08038248
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02003c28
	.thumb_func
Func_02003c28:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02003c30
	.thumb_func
Func_02003c30:
	overlay_veneer EventRuntime_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
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
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Func_02003c68
	.thumb_func
Func_02003c68:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02003c70
	.thumb_func
Func_02003c70:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02003c78
	.thumb_func
Func_02003c78:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02003cb0
	.thumb_func
Func_02003cb0:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02003cc0
	.thumb_func
Func_02003cc0:
	overlay_veneer Func_080c8208
	.global Func_02003cc8
	.thumb_func
Func_02003cc8:
	overlay_veneer Func_080c8210
	.global Func_02003cd0
	.thumb_func
Func_02003cd0:
	overlay_veneer Func_080c8218
	.global ObjectVisual_CopyAttributes
	.thumb_func
ObjectVisual_CopyAttributes:
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global Func_02003ce0
	.thumb_func
Func_02003ce0:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003cf0
	.thumb_func
Func_02003cf0:
	overlay_veneer Func_080c8240
	.global Func_02003cf8
	.thumb_func
Func_02003cf8:
	overlay_veneer Func_080c8258
	.global Func_02003d00
	.thumb_func
Func_02003d00:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02003d08
	.thumb_func
Func_02003d08:
	overlay_veneer Func_080c8288
	.global Func_02003d10
	.thumb_func
Func_02003d10:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	overlay_veneer Func_080c82c0
	.endif
	.endif
	.global Func_02003d18
	.thumb_func
Func_02003d18:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c82c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c82c8
	.else
	overlay_veneer BattleEffect_FindNearestObjectFar
	.endif
	.endif
	.global Func_02003d20
	.thumb_func
Func_02003d20:
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
	.global Func_02003d40
	.thumb_func
Func_02003d40:
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayScroll_DisarmHBlankDmaFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayScroll_DisarmHBlankDmaFar
	.else
	overlay_veneer Func_080c83d8
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
	.global Func_02003d50
	.thumb_func
Func_02003d50:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8490
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8490
	.else
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.endif
	.endif
	.global Func_02003d58
	.thumb_func
Func_02003d58:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	overlay_veneer BattleEffect_SetupObjectPairFar
	.endif
	.endif
	.global Func_02003d60
	.thumb_func
Func_02003d60:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.else
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.endif
	.endif
	.global Func_02003d68
	.thumb_func
Func_02003d68:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetupObjectPairFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetupObjectPairFar
	.else
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.endif
	.endif
	.global Func_02003d70
	.thumb_func
Func_02003d70:
	.ifdef TLA_EDITION_ES
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.else
	overlay_veneer EventObject_InitializeFar
	.endif
	.endif
	.global Func_02003d78
	.thumb_func
Func_02003d78:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02003d80
	.thumb_func
Func_02003d80:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_HasMatchingEvent5Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_HasMatchingEvent5Far
	.else
	overlay_veneer Func_080c8518
	.endif
	.endif
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02003d90
	.thumb_func
Func_02003d90:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8698
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8698
	.else
	overlay_veneer Func_080c86a8
	.endif
	.endif
	.global Func_02003d98
	.thumb_func
Func_02003d98:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c0
	.else
	overlay_veneer Func_080c86d0
	.endif
	.endif
	.global Func_02003da0
	.thumb_func
Func_02003da0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c8
	.else
	overlay_veneer Func_080c86d8
	.endif
	.endif
	.global Func_02003da8
	.thumb_func
Func_02003da8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86d8
	.else
	overlay_veneer Func_080c86e8
	.endif
	.endif
	.global Func_02003db0
	.thumb_func
Func_02003db0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_02003db8
	.thumb_func
Func_02003db8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8708
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8708
	.else
	overlay_veneer Func_080c8718
	.endif
	.endif
	.global Func_02003dc0
	.thumb_func
Func_02003dc0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8710
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8710
	.else
	overlay_veneer Func_080c8720
	.endif
	.endif
	.global Func_02003dc8
	.thumb_func
Func_02003dc8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8718
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8718
	.else
	overlay_veneer Func_080c8728
	.endif
	.endif
	.global Func_02003dd0
	.thumb_func
Func_02003dd0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8720
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8720
	.else
	overlay_veneer Func_080c8730
	.endif
	.endif
	.global Func_02003dd8
	.thumb_func
Func_02003dd8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8728
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8728
	.else
	overlay_veneer Func_080c8738
	.endif
	.endif
	.global Func_02003de0
	.thumb_func
Func_02003de0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8730
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8730
	.else
	overlay_veneer Func_080c8740
	.endif
	.endif
	.global Func_02003de8
	.thumb_func
Func_02003de8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8738
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8738
	.else
	overlay_veneer Func_080c8748
	.endif
	.endif
	.global Func_02003df0
	.thumb_func
Func_02003df0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8740
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8740
	.else
	overlay_veneer Func_080c8750
	.endif
	.endif
	.global Func_02003df8
	.thumb_func
Func_02003df8:
	overlay_veneer Audio_PlayCue
