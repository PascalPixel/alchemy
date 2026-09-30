@ Import veneers after the OVERLAY_660 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_660 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Func_08020120
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08020240
	overlay_veneer Func_08020248
	overlay_veneer Func_080202a0
	overlay_veneer Func_08038210
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8070
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c80a0
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8100
	overlay_veneer Func_080c8128
	overlay_veneer Func_080c8148
	overlay_veneer Func_080c8158
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	overlay_veneer Func_080c8198
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c8480
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8778
	overlay_veneer Func_080c87a8
	overlay_veneer Func_08108008
	overlay_veneer Func_08108010
	overlay_veneer Func_08108018
	overlay_veneer Audio_PlayCue
