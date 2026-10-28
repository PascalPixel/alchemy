.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
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
	overlay_veneer Actor_SetMode
	.global Func_02000548
	.thumb_func
Func_02000548:
	overlay_veneer Actor_StartScriptFar
	.global Func_02000550
	.thumb_func
Func_02000550:
	overlay_veneer Func_080200c0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02000568
	.thumb_func
Func_02000568:
	overlay_veneer Event_StartFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02000598
	.thumb_func
Func_02000598:
	overlay_veneer Event_SetMessageIdFar
	.global Func_020005a0
	.thumb_func
Func_020005a0:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_020005b0
	.thumb_func
Func_020005b0:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_020005c0
	.thumb_func
Func_020005c0:
	overlay_veneer Func_080c8210
	.global Func_020005c8
	.thumb_func
Func_020005c8:
	overlay_veneer Event_SetExitRequestFar
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
	overlay_veneer Event_ResolvePendingActionsFar
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
