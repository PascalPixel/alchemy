@ Far-call stub table at 0x080f8090: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080f8090
	.thumb_func
Func_080f8090:
	overlay_veneer Func_08105300
	.global Func_080f8098
	.thumb_func
Func_080f8098:
	overlay_veneer Func_0810532c
	.global Func_080f80a0
	.thumb_func
Func_080f80a0:
	overlay_veneer Func_08105350
