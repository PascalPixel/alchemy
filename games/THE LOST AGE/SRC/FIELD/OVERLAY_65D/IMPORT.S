@ Import veneers after the OVERLAY_65D overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_65D overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_08020170
	overlay_veneer Func_08020178
	overlay_veneer Func_080201a0
	overlay_veneer Func_080201e0
	overlay_veneer Func_080201e8
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8070
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c8098
	overlay_veneer Func_080c80d8
	overlay_veneer Func_080c80e0
	overlay_veneer Func_080c80e8
	overlay_veneer Func_080c80f0
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c83b0
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8580
	overlay_veneer Func_080c85f8
	overlay_veneer Func_080c8778
	overlay_veneer Audio_PlayCue
