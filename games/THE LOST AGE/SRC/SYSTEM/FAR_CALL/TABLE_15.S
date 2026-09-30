@ Far-call stub table at 0x080381c0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080381c0
	.thumb_func
Func_080381c0:
	overlay_veneer Func_0804bc08
	overlay_veneer Func_0803a5e0
	overlay_veneer Func_0803a54c
	overlay_veneer Func_0803f620
