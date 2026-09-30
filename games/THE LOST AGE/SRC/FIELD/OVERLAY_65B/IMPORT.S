@ Import veneers after the OVERLAY_65B overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_65B overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_02000298
	.thumb_func
Func_02000298:
	overlay_veneer Func_080003c8
	.global Func_020002a0
	.thumb_func
Func_020002a0:
	overlay_veneer Func_080003d0
	.global Func_020002a8
	.thumb_func
Func_020002a8:
	overlay_veneer Func_080201e8
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020002b8
	.thumb_func
Func_020002b8:
	overlay_veneer Func_080c8018
	.global Func_020002c0
	.thumb_func
Func_020002c0:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_02000300
	.thumb_func
Func_02000300:
	overlay_veneer Func_080c8180
	.global Func_02000308
	.thumb_func
Func_02000308:
	overlay_veneer Func_080c8188
	.global Func_02000310
	.thumb_func
Func_02000310:
	overlay_veneer Func_080c8198
	.global Func_02000318
	.thumb_func
Func_02000318:
	overlay_veneer Func_080c81a0
	.global Func_02000320
	.thumb_func
Func_02000320:
	overlay_veneer Func_080c81d0
	.global Func_02000328
	.thumb_func
Func_02000328:
	overlay_veneer Func_080c8230
	.global Func_02000330
	.thumb_func
Func_02000330:
	overlay_veneer Func_080c8238
	.global Func_02000338
	.thumb_func
Func_02000338:
	overlay_veneer Func_080c8240
	.global Func_02000340
	.thumb_func
Func_02000340:
	overlay_veneer Func_080c84e0
