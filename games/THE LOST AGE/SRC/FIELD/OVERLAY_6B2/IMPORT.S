@ Import veneers after the OVERLAY_6B2 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B2 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer WaitFramesFar
	overlay_veneer Func_080000d0
	overlay_veneer Func_080000f8
	overlay_veneer Func_080003d0
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetCallback
	overlay_veneer Func_080200c0
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Object_GetByIdFar
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	overlay_veneer ObjectMotion_LaunchFar
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c81d8
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c8378
	overlay_veneer Func_080c8390
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b0
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c84e8
	overlay_veneer Func_080c84f0
	overlay_veneer Audio_PlayCue
