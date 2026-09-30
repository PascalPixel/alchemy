@ Far-call stub table at 0x080ad2e8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080ad2e8
	.thumb_func
Func_080ad2e8:
	overlay_veneer Func_080af464
	.global Func_080ad2f0
	.thumb_func
Func_080ad2f0:
	overlay_veneer Func_080aee40
	.global Func_080ad2f8
	.thumb_func
Func_080ad2f8:
	overlay_veneer Func_080aff94
	overlay_veneer Func_080b1004
	.global Func_080ad308
	.thumb_func
Func_080ad308:
	overlay_veneer Func_080af0e4
	.global Func_080ad310
	.thumb_func
Func_080ad310:
	overlay_veneer Func_080add5c
