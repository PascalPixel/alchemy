@ Import veneers after the OVERLAY_65C overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_65C overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_02000170
	.thumb_func
Func_02000170:
	overlay_veneer Func_080000d0
	.global Func_02000178
	.thumb_func
Func_02000178:
	overlay_veneer Func_080003d0
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02000190
	.thumb_func
Func_02000190:
	overlay_veneer Func_080c86a8
	.global Func_02000198
	.thumb_func
Func_02000198:
	overlay_veneer Func_080c86b0
	.global Func_020001a0
	.thumb_func
Func_020001a0:
	overlay_veneer Func_080c86c0
	.global Func_020001a8
	.thumb_func
Func_020001a8:
	overlay_veneer Func_080c86e8
