@ Far-call stub table at 0x080c8648: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8648
	.thumb_func
Func_080c8648:
	overlay_veneer Func_080ca280
	overlay_veneer Func_080d1ee8
	overlay_veneer Func_080d1f0c
	overlay_veneer Func_080d1f18
	overlay_veneer Func_080d1f20
	overlay_veneer Func_080d2084
	overlay_veneer Func_080d20fc
	.global Func_080c8680
	.thumb_func
Func_080c8680:
	overlay_veneer Object_SetActionCallback
	.global Func_080c8688
	.thumb_func
Func_080c8688:
	overlay_veneer Func_080d2c64
	.global Func_080c8690
	.thumb_func
Func_080c8690:
	overlay_veneer Func_080d2c98
