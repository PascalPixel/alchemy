@ Draft: original 64-byte import bank for the 820-byte ES Madora scene.
@ Complete loaded scene differs in four import-word bytes: initialize, set
@ placement, join, and flag-selected placement resolve to the wrong native
@ byte owners. No instruction or scalar differs in the four complete callees.
@ Import veneers after the OVERLAY_65C overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_65C overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
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
