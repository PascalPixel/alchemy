@ Far-call stub table at 0x080ad200: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Owner_GetLevelThresholdFar
	.thumb_func
Owner_GetLevelThresholdFar:
	overlay_veneer Owner_GetLevelThreshold
	.global Func_080ad208
	.thumb_func
Func_080ad208:
	overlay_veneer Func_080ae16c
	.global Func_080ad210
	.thumb_func
Func_080ad210:
	overlay_veneer Func_080ae220
	.global Func_080ad218
	.thumb_func
Func_080ad218:
	overlay_veneer Func_080ae0dc
	.global Func_080ad220
	.thumb_func
Func_080ad220:
	overlay_veneer Func_080ae0f0
	.global Func_080ad228
	.thumb_func
Func_080ad228:
	overlay_veneer Func_080ae118
	.global Func_080ad230
	.thumb_func
Func_080ad230:
	overlay_veneer Func_080ae830
	.global Func_080ad238
	.thumb_func
Func_080ad238:
	overlay_veneer Func_080ae410
	.global Func_080ad240
	.thumb_func
Func_080ad240:
	overlay_veneer Func_080ae6e4
