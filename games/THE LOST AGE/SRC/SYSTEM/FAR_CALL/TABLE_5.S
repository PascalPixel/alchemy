@ Far-call stub table at 0x080201f0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080201f0
	.thumb_func
Func_080201f0:
	overlay_veneer Func_0802b450
	.global Func_080201f8
	.thumb_func
Func_080201f8:
	overlay_veneer Func_0802b590
	.global Func_08020200
	.thumb_func
Func_08020200:
	overlay_veneer Func_0802bd08
	.global Func_08020208
	.thumb_func
Func_08020208:
	overlay_veneer Func_0802bdd0
