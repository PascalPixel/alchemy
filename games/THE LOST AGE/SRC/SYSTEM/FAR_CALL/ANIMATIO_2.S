@ Far-call stub table at 0x08020238: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Animation_SetIndexAndInitObjectsFar
	.thumb_func
Animation_SetIndexAndInitObjectsFar:
	overlay_veneer Animation_SetIndexAndInitObjects
	.global Func_08020240
	.thumb_func
Func_08020240:
	overlay_veneer Func_080237f8
	.global Func_08020248
	.thumb_func
Func_08020248:
	overlay_veneer Func_08023840
	.global Animation_SetDisplayFlagFar
	.thumb_func
Animation_SetDisplayFlagFar:
	overlay_veneer Animation_SetDisplayFlag
