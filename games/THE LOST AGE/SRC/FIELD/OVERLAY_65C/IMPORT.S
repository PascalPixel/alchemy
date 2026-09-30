@ Import veneers after the OVERLAY_65C overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_65C overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080000d0
	overlay_veneer Func_080003d0
	overlay_veneer Object_GetByIdFar
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Func_080c86a8
	overlay_veneer Func_080c86b0
	overlay_veneer Func_080c86c0
	overlay_veneer Func_080c86e8
