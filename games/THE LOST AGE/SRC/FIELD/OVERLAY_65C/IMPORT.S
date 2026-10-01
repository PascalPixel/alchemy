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
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8698
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8698
	.else
	overlay_veneer Func_080c86a8
	.endif
	.endif
	.global Func_02000198
	.thumb_func
Func_02000198:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86a0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86a0
	.else
	overlay_veneer Func_080c86b0
	.endif
	.endif
	.global Func_020001a0
	.thumb_func
Func_020001a0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86b0
	.else
	overlay_veneer Func_080c86c0
	.endif
	.endif
	.global Func_020001a8
	.thumb_func
Func_020001a8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86d8
	.else
	overlay_veneer Func_080c86e8
	.endif
	.endif
