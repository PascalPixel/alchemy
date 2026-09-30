@ Import veneers after the OVERLAY_677 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_677 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08038120
	overlay_veneer Func_080ad010
	overlay_veneer Func_080ad038
	overlay_veneer Func_080ad1d8
	overlay_veneer Func_080ad2f0
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8060
	overlay_veneer Func_080c8070
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c8098
	overlay_veneer Func_080c80c0
	overlay_veneer Func_080c80d0
	overlay_veneer Func_080c80f0
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8140
	overlay_veneer Func_080c8158
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c83e0
	overlay_veneer Func_080c8480
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8778
	overlay_veneer Func_080c87b0
	overlay_veneer Func_08108008
	overlay_veneer Func_08108010
	overlay_veneer Func_08108018
	overlay_veneer Func_08108020
	overlay_veneer Func_08108098
	overlay_veneer Func_081080a0
