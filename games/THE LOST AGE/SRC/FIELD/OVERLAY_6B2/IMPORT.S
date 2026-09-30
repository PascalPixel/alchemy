@ Import veneers after the OVERLAY_6B2 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B2 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02000528
	.thumb_func
Func_02000528:
	overlay_veneer Func_080000d0
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Func_02000538
	.thumb_func
Func_02000538:
	overlay_veneer Func_080003d0
	.global Func_02000540
	.thumb_func
Func_02000540:
	overlay_veneer Object_SetMode
	.global Func_02000548
	.thumb_func
Func_02000548:
	overlay_veneer Object_SetCallback
	.global Func_02000550
	.thumb_func
Func_02000550:
	overlay_veneer Func_080200c0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02000568
	.thumb_func
Func_02000568:
	overlay_veneer Func_080c8018
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
	.global Func_02000590
	.thumb_func
Func_02000590:
	overlay_veneer Func_080c8170
	.global Func_02000598
	.thumb_func
Func_02000598:
	overlay_veneer Func_080c8180
	.global Func_020005a0
	.thumb_func
Func_020005a0:
	overlay_veneer Func_080c81a0
	.global Func_020005a8
	.thumb_func
Func_020005a8:
	overlay_veneer Func_080c81d0
	.global Func_020005b0
	.thumb_func
Func_020005b0:
	overlay_veneer Func_080c81d8
	.global Func_020005b8
	.thumb_func
Func_020005b8:
	overlay_veneer Func_080c8200
	.global Func_020005c0
	.thumb_func
Func_020005c0:
	overlay_veneer Func_080c8210
	.global Func_020005c8
	.thumb_func
Func_020005c8:
	overlay_veneer Func_080c8278
	.global Func_020005d0
	.thumb_func
Func_020005d0:
	overlay_veneer Func_080c8378
	.global Func_020005d8
	.thumb_func
Func_020005d8:
	overlay_veneer Func_080c8390
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
	.global Func_020005f8
	.thumb_func
Func_020005f8:
	overlay_veneer Func_080c84e0
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
	overlay_veneer Audio_PlayCue
