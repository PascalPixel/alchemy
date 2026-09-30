@ Far-call stub table at 0x080ad0b0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080ad0b0
	.thumb_func
Func_080ad0b0:
	overlay_veneer Func_080afb80
	overlay_veneer Func_080ad338
