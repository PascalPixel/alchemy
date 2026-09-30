@ Import veneers after the OVERLAY_66E overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_66E overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramUnsignedRemainder
	overlay_veneer Func_080000f8
	overlay_veneer Func_08000148
	overlay_veneer Func_08000150
	overlay_veneer Func_080003c0
	overlay_veneer Func_080003c8
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08020300
	overlay_veneer Func_08038040
	overlay_veneer Func_08038120
	overlay_veneer Func_080ad010
	overlay_veneer Func_080ad038
	overlay_veneer Func_080ad1d8
	overlay_veneer Func_080ad2e8
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8070
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c80d0
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8128
	overlay_veneer Func_080c8130
	overlay_veneer Func_080c8138
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8208
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8378
	overlay_veneer Func_080c8380
	overlay_veneer Func_080c8390
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8778
	overlay_veneer Func_08108008
	overlay_veneer Func_08108018
	overlay_veneer Func_08108070
	overlay_veneer Audio_PlayCue
