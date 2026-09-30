@ Import veneers after the OVERLAY_688 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_688 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02000504
	.thumb_func
Func_02000504:
	overlay_veneer Func_080003c8
	.global Func_0200050c
	.thumb_func
Func_0200050c:
	overlay_veneer Func_080003d0
	.global Func_02000514
	.thumb_func
Func_02000514:
	overlay_veneer Func_080003d8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global UiText_OpenMessageWindow
	.thumb_func
UiText_OpenMessageWindow:
	overlay_veneer UiText_OpenMessageWindowFar
	.global UiWork_IsComplete
	.thumb_func
UiWork_IsComplete:
	overlay_veneer UiWork_IsCompleteFar
	.global Func_0200053c
	.thumb_func
Func_0200053c:
	overlay_veneer Func_080380f8
	.global Func_02000544
	.thumb_func
Func_02000544:
	overlay_veneer Func_08038108
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_0200055c
	.thumb_func
Func_0200055c:
	overlay_veneer Func_080c8018
	.global Func_02000564
	.thumb_func
Func_02000564:
	overlay_veneer Func_080c8020
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
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global Func_0200058c
	.thumb_func
Func_0200058c:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_0200059c
	.thumb_func
Func_0200059c:
	overlay_veneer Func_080c8180
	.global Func_020005a4
	.thumb_func
Func_020005a4:
	overlay_veneer Func_080c8268
	.global Func_020005ac
	.thumb_func
Func_020005ac:
	overlay_veneer Func_080c8278
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	overlay_veneer Func_080c84e0
	.global BattleFx_GetResourceId
	.thumb_func
BattleFx_GetResourceId:
	overlay_veneer BattleFx_GetResourceIdFar
	.global Func_020005dc
	.thumb_func
Func_020005dc:
	overlay_veneer Audio_PlayCue
