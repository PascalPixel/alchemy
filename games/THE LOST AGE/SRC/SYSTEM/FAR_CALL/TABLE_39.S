@ Far-call stub table at 0x080f8058: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080f8058
	.thumb_func
Func_080f8058:
	overlay_veneer Func_08104ef8
	overlay_veneer Func_08104fe0
	overlay_veneer Func_0810508c
	overlay_veneer Func_081051a8
	overlay_veneer Func_0810526c
	overlay_veneer Func_081050b8
	overlay_veneer Func_081052ac
