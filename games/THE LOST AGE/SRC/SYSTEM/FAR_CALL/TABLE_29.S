@ Far-call stub table at 0x080c84d8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c84d8
	.thumb_func
Func_080c84d8:
	overlay_veneer Func_080cdec8
	.global Func_080c84e0
	.thumb_func
Func_080c84e0:
	overlay_veneer Func_080cded4
	.global Func_080c84e8
	.thumb_func
Func_080c84e8:
	overlay_veneer Func_080dc978
	.global Func_080c84f0
	.thumb_func
Func_080c84f0:
	overlay_veneer Func_080dca50
	.global Func_080c84f8
	.thumb_func
Func_080c84f8:
	overlay_veneer Func_080dca84
	.global Func_080c8500
	.thumb_func
Func_080c8500:
	overlay_veneer Func_080dcadc
