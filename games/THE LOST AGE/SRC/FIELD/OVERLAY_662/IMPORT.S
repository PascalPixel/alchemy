@ Import veneers after the OVERLAY_662 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_662 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080000c0
	overlay_veneer Func_080000d0
	overlay_veneer Func_080000d8
	overlay_veneer Func_08000128
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Func_080003e8
	overlay_veneer Func_080003f0
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetPositionAndResetMotionFar
	overlay_veneer Object_SetPosition
	overlay_veneer Object_CommitPosition
	overlay_veneer Func_080201e8
	overlay_veneer Func_08020210
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08038210
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c8098
	overlay_veneer Func_080c80c0
	overlay_veneer Func_080c80e8
	overlay_veneer Func_080c80f0
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8140
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8208
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8268
	overlay_veneer Func_080c8280
	overlay_veneer Func_080c8288
	overlay_veneer Func_080c8378
	overlay_veneer Func_080c8380
	overlay_veneer Func_080c8390
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c8420
	overlay_veneer Func_080c8428
	overlay_veneer Func_080c8480
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8690
	overlay_veneer Func_080c8710
	overlay_veneer Func_080c8738
	overlay_veneer Func_080c88d8
	overlay_veneer Func_080c88e8
	overlay_veneer Audio_PlayCue
