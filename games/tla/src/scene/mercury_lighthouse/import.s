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
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02000540
	.thumb_func
Func_02000540:
	overlay_veneer Object_SetMode
	.global Func_02000548
	.thumb_func
Func_02000548:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02000550
	.thumb_func
Func_02000550:
	overlay_veneer Func_080200c0
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02000568
	.thumb_func
Func_02000568:
	overlay_veneer EventRuntime_StartFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02000598
	.thumb_func
Func_02000598:
	overlay_veneer EventRuntime_SetMessageFar
	.global Func_020005a0
	.thumb_func
Func_020005a0:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_020005b0
	.thumb_func
Func_020005b0:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_020005c0
	.thumb_func
Func_020005c0:
	overlay_veneer Func_080c8210
	.global Func_020005c8
	.thumb_func
Func_020005c8:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_020005d0
	.thumb_func
Func_020005d0:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_020005d8
	.thumb_func
Func_020005d8:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_020005f8
	.thumb_func
Func_020005f8:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02000600
	.thumb_func
Func_02000600:
	overlay_veneer Func_080c84e8
	.global Func_02000608
	.thumb_func
Func_02000608:
	overlay_veneer Func_080c84f0
	.global Func_02000610
	.thumb_func
Func_02000610:
	overlay_veneer Sound_PlayFar
