@ Import veneers after the OVERLAY_66C overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_66C overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_0200019c
	.thumb_func
Func_0200019c:
	overlay_veneer Func_080003c8
	.global Func_020001a4
	.thumb_func
Func_020001a4:
	overlay_veneer Func_080003d0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_020001b4
	.thumb_func
Func_020001b4:
	overlay_veneer Func_080c8018
	.global Func_020001bc
	.thumb_func
Func_020001bc:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Func_020001dc
	.thumb_func
Func_020001dc:
	overlay_veneer Func_080c8158
	.global Func_020001e4
	.thumb_func
Func_020001e4:
	overlay_veneer Func_080c8180
	.global Func_020001ec
	.thumb_func
Func_020001ec:
	overlay_veneer Func_080c81a0
	.global Func_020001f4
	.thumb_func
Func_020001f4:
	overlay_veneer Func_080c8218
	.global Func_020001fc
	.thumb_func
Func_020001fc:
	overlay_veneer Func_080c8228
	.global Func_02000204
	.thumb_func
Func_02000204:
	overlay_veneer Func_080c8230
	.global Func_0200020c
	.thumb_func
Func_0200020c:
	overlay_veneer Func_080c8240
	.global Func_02000214
	.thumb_func
Func_02000214:
	overlay_veneer Func_080c84e0
	.global Func_0200021c
	.thumb_func
Func_0200021c:
	overlay_veneer Func_080c8778
