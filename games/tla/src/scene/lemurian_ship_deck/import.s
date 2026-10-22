.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
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
	.global Func_02001efc
	.thumb_func
Func_02001efc:
	overlay_veneer Object_SetMode
	.global Func_02001f04
	.thumb_func
Func_02001f04:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02001f0c
	.thumb_func
Func_02001f0c:
	overlay_veneer Func_080200c0
	.global Func_02001f14
	.thumb_func
Func_02001f14:
	overlay_veneer Object_Destroy
	.global Func_02001f1c
	.thumb_func
Func_02001f1c:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02001f24
	.thumb_func
Func_02001f24:
	overlay_veneer Object_SetPosition
	.global Func_02001f2c
	.thumb_func
Func_02001f2c:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Func_02001f34
	.thumb_func
Func_02001f34:
	.ifdef TLA_EDITION_IT
	overlay_veneer MapLayer_ClearField22Far
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer MapLayer_ClearField22Far
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer MapLayer_ClearField22Far
	.else
	.ifdef TLA_EDITION_DE
	overlay_veneer MapLayer_ClearField22Far
	.else
	overlay_veneer Func_08020198
	.endif
	.endif
	.endif
	.endif
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02001f4c
	.thumb_func
Func_02001f4c:
	overlay_veneer Func_08038248
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001f5c
	.thumb_func
Func_02001f5c:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02001f64
	.thumb_func
Func_02001f64:
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
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer Object_SetActionCallbackAndRefreshByIdFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_CenterOnColumn
	.thumb_func
ObjectMotion_CenterOnColumn:
	overlay_veneer ObjectMotion_CenterOnColumnFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02001fc4
	.thumb_func
Func_02001fc4:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02001fcc
	.thumb_func
Func_02001fcc:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02001fd4
	.thumb_func
Func_02001fd4:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_0200201c
	.thumb_func
Func_0200201c:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_0200202c
	.thumb_func
Func_0200202c:
	overlay_veneer Func_080c8198
	.global Func_02002034
	.thumb_func
Func_02002034:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02002044
	.thumb_func
Func_02002044:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global Func_0200204c
	.thumb_func
Func_0200204c:
	overlay_veneer Func_080c8210
	.global Func_02002054
	.thumb_func
Func_02002054:
	overlay_veneer Func_080c8218
	.global Func_0200205c
	.thumb_func
Func_0200205c:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_0200206c
	.thumb_func
Func_0200206c:
	overlay_veneer Func_080c8240
	.global Func_02002074
	.thumb_func
Func_02002074:
	overlay_veneer Func_080c8258
	.global Func_0200207c
	.thumb_func
Func_0200207c:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02002084
	.thumb_func
Func_02002084:
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8368
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8368
	.else
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.endif
	.endif
	.global Func_0200208c
	.thumb_func
Func_0200208c:
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.else
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.endif
	.endif
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	.ifdef TLA_EDITION_IT
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	overlay_veneer Event_StartScreenTransitionFar
	.endif
	.endif
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetPrimaryBufferValueFar
	.else
	.ifdef TLA_EDITION_ES
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
	.global Func_020020ac
	.thumb_func
Func_020020ac:
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_020020b4
	.thumb_func
Func_020020b4:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_020020c4
	.thumb_func
Func_020020c4:
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86e8
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86e8
	.else
	overlay_veneer Func_080c86f8
	.endif
	.endif
	.global Func_020020cc
	.thumb_func
Func_020020cc:
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86f8
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86f8
	.else
	overlay_veneer Func_080c8708
	.endif
	.endif
	.global Func_020020d4
	.thumb_func
Func_020020d4:
	overlay_veneer Audio_PlayCue
