@ Import veneers after the OVERLAY_688 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_688 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080000c0
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer UiWork_FinalizeFar
	overlay_veneer Func_08038038
	overlay_veneer Func_08038048
	overlay_veneer Func_080380f8
	overlay_veneer Func_08038108
	overlay_veneer Func_08038140
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c8098
	overlay_veneer Func_080c80a0
	overlay_veneer Func_080c80a8
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8268
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b0
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c85c8
	overlay_veneer Audio_PlayCue
