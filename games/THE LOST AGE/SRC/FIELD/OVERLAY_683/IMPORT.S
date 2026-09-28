@ Import veneers after the OVERLAY_683 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_683 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d8
	overlay_veneer Func_08108008
	overlay_veneer Func_08108018
