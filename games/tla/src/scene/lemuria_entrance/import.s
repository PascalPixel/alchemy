.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
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
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Window_Close
	.thumb_func
Window_Close:
	overlay_veneer Window_CloseFar
	.global Message_OpenWindow
	.thumb_func
Message_OpenWindow:
	overlay_veneer Message_OpenWindowFar
	.global Message_AllTyped
	.thumb_func
Message_AllTyped:
	overlay_veneer Message_AllTypedFar
	.global Func_0200053c
	.thumb_func
Func_0200053c:
	overlay_veneer Func_080380f8
	.global Text_GetMessageSize
	.thumb_func
Text_GetMessageSize:
	overlay_veneer Text_GetMessageSizeFar
	.global Message_CloseFinishedWindows
	.thumb_func
Message_CloseFinishedWindows:
	overlay_veneer Message_CloseFinishedWindowsFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_0200055c
	.thumb_func
Func_0200055c:
	overlay_veneer Event_StartFar
	.global Func_02000564
	.thumb_func
Func_02000564:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_RefreshSelectorById
	.thumb_func
Actor_RefreshSelectorById:
	overlay_veneer Actor_RefreshSelectorByIdFar
	.global Func_0200058c
	.thumb_func
Func_0200058c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Func_0200059c
	.thumb_func
Func_0200059c:
	overlay_veneer Event_SetMessageIdFar
	.global Func_020005a4
	.thumb_func
Func_020005a4:
	overlay_veneer Event_SetDestinationFar
	.global Func_020005ac
	.thumb_func
Func_020005ac:
	overlay_veneer Event_SetExitRequestFar
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
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	overlay_veneer Event_ResolvePendingActionsFar
	.global BattleEffect_GetResourceId
	.thumb_func
BattleEffect_GetResourceId:
	overlay_veneer BattleEffect_GetResourceIdFar
	.global Func_020005dc
	.thumb_func
Func_020005dc:
	overlay_veneer Sound_PlayFar
