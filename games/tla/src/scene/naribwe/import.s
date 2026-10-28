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
	.global Func_02000534
	.thumb_func
Func_02000534:
	overlay_veneer Map_PlayMapEditFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_0200054c
	.thumb_func
Func_0200054c:
	overlay_veneer EventRuntime_StartFar
	.global Func_02000554
	.thumb_func
Func_02000554:
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
	.global Func_02000574
	.thumb_func
Func_02000574:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02000584
	.thumb_func
Func_02000584:
	overlay_veneer EventRuntime_SetMessageFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02000594
	.thumb_func
Func_02000594:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_0200059c
	.thumb_func
Func_0200059c:
	overlay_veneer Func_080c8208
	.global Func_020005a4
	.thumb_func
Func_020005a4:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_020005bc
	.thumb_func
Func_020005bc:
	overlay_veneer ObjectEvent_PlaceOnGroundFar
	.global Func_020005c4
	.thumb_func
Func_020005c4:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	overlay_veneer Func_080c86a8
	.global Func_020005d4
	.thumb_func
Func_020005d4:
	overlay_veneer Func_080c86e8
	.global Func_020005dc
	.thumb_func
Func_020005dc:
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.global Func_020005e4
	.thumb_func
Func_020005e4:
	overlay_veneer Func_08108008
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	overlay_veneer Func_08108010
	.global Func_020005f4
	.thumb_func
Func_020005f4:
	overlay_veneer Sound_PlayFar
