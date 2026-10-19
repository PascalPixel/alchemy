.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
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
	.global Func_020007bc
	.thumb_func
Func_020007bc:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_020007cc
	.thumb_func
Func_020007cc:
	overlay_veneer Func_08020240
	.global Func_020007d4
	.thumb_func
Func_020007d4:
	overlay_veneer Func_08020248
	.global Func_020007dc
	.thumb_func
Func_020007dc:
	overlay_veneer Func_080202a0
	.global Func_020007e4
	.thumb_func
Func_020007e4:
	overlay_veneer Func_08038210
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020007f4
	.thumb_func
Func_020007f4:
	overlay_veneer EventRuntime_BeginFar
	.global Func_020007fc
	.thumb_func
Func_020007fc:
	overlay_veneer EventRuntime_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Func_0200081c
	.thumb_func
Func_0200081c:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02000824
	.thumb_func
Func_02000824:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_02000844
	.thumb_func
Func_02000844:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02000854
	.thumb_func
Func_02000854:
	overlay_veneer Func_080c8198
	.global Func_0200085c
	.thumb_func
Func_0200085c:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
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
	.global Func_0200087c
	.thumb_func
Func_0200087c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global Func_02000884
	.thumb_func
Func_02000884:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_0200088c
	.thumb_func
Func_0200088c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.endif
	.endif
	.global Func_02000894
	.thumb_func
Func_02000894:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8798
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8798
	.else
	overlay_veneer Func_080c87a8
	.endif
	.endif
	.global Func_0200089c
	.thumb_func
Func_0200089c:
	overlay_veneer Func_08108008
	.global Func_020008a4
	.thumb_func
Func_020008a4:
	overlay_veneer Func_08108010
	.global Func_020008ac
	.thumb_func
Func_020008ac:
	overlay_veneer Func_08108018
	.global Func_020008b4
	.thumb_func
Func_020008b4:
	overlay_veneer Audio_PlayCue
