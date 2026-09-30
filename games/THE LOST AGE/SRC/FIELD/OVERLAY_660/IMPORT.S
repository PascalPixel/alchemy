@ Import veneers after the OVERLAY_660 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_660 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
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
	overlay_veneer Func_08020120
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
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
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020007f4
	.thumb_func
Func_020007f4:
	overlay_veneer Func_080c8018
	.global Func_020007fc
	.thumb_func
Func_020007fc:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
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
	overlay_veneer Func_080c80f8
	.global Func_02000824
	.thumb_func
Func_02000824:
	overlay_veneer Func_080c8100
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
	overlay_veneer Func_080c8180
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
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_0200087c
	.thumb_func
Func_0200087c:
	overlay_veneer Func_080c8480
	.global Func_02000884
	.thumb_func
Func_02000884:
	overlay_veneer Func_080c84e0
	.global Func_0200088c
	.thumb_func
Func_0200088c:
	overlay_veneer Func_080c8778
	.global Func_02000894
	.thumb_func
Func_02000894:
	overlay_veneer Func_080c87a8
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
