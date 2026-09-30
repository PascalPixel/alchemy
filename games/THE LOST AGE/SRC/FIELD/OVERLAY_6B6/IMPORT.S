@ Import veneers after the OVERLAY_6B6 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B6 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_020000b4
	.thumb_func
Func_020000b4:
	overlay_veneer Func_080003d8
	.global Func_020000bc
	.thumb_func
Func_020000bc:
	overlay_veneer Func_080c8580
