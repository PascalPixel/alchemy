@ Import veneers after the OVERLAY_683 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_683 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_02000168
	.thumb_func
Func_02000168:
	overlay_veneer Func_080003c8
	.global Func_02000170
	.thumb_func
Func_02000170:
	overlay_veneer Func_080003d8
	.global Func_02000178
	.thumb_func
Func_02000178:
	overlay_veneer Func_08108008
	.global Func_02000180
	.thumb_func
Func_02000180:
	overlay_veneer Func_08108018
