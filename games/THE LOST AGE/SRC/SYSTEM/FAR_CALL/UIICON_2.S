@ Far-call stub table at 0x080f8028: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080f8028
	.thumb_func
Func_080f8028:
	overlay_veneer UiIcon_PrepareObject
	.global Func_080f8030
	.thumb_func
Func_080f8030:
	overlay_veneer Func_08100e7c
	.global Func_080f8038
	.thumb_func
Func_080f8038:
	overlay_veneer Func_080fb8ac
	overlay_veneer Func_080fb8b8
	overlay_veneer Func_080f92ac
	overlay_veneer Func_08104da8
