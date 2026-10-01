@ Far-call stub table at 0x080c8390: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8390
	.thumb_func
Func_080c8390:
	overlay_veneer BattleFx_StartBufferInterpolation
	.global Func_080c8398
	.thumb_func
Func_080c8398:
	overlay_veneer Func_080d16f8
	.global Func_080c83a0
	.thumb_func
Func_080c83a0:
	overlay_veneer Func_080d174c
