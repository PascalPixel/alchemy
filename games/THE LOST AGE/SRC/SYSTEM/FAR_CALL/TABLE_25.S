@ Far-call stub table at 0x080c8268: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8268
	.thumb_func
Func_080c8268:
	overlay_veneer Func_080d2aa4
	overlay_veneer Func_080d2ad0
	.global Func_080c8278
	.thumb_func
Func_080c8278:
	overlay_veneer Func_080d2afc
	.global Func_080c8280
	.thumb_func
Func_080c8280:
	overlay_veneer Func_080d2b0c
	.global Func_080c8288
	.thumb_func
Func_080c8288:
	overlay_veneer Func_080d2b4c
	.global Func_080c8290
	.thumb_func
Func_080c8290:
	overlay_veneer Func_080d2c04
	.global Func_080c8298
	.thumb_func
Func_080c8298:
	overlay_veneer Func_080d2c1c
	.global Func_080c82a0
	.thumb_func
Func_080c82a0:
	overlay_veneer Func_080d2c34
