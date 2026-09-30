@ Import veneers after the OVERLAY_6B9 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B9 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080000c0
	overlay_veneer Func_080000d0
	overlay_veneer Func_080000d8
	overlay_veneer Func_080000f8
	overlay_veneer Func_08000128
	overlay_veneer Func_08000140
	overlay_veneer Func_08000150
	overlay_veneer Func_080001a8
	overlay_veneer Func_080001b8
	overlay_veneer Func_080001c8
	overlay_veneer Func_080001d0
	overlay_veneer Func_08000290
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080200c0
	overlay_veneer Func_080201e0
	overlay_veneer Func_080201e8
	overlay_veneer Func_080201f0
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8280
	overlay_veneer Func_080c86a8
	overlay_veneer Func_080c86b8
	overlay_veneer Func_080c87c0
	overlay_veneer Func_080c87d0
	overlay_veneer Func_080c87e0
