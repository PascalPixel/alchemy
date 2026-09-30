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
	.global Func_080f8060
	.thumb_func
Func_080f8060:
	overlay_veneer Func_08104fe0
	.global Func_080f8068
	.thumb_func
Func_080f8068:
	overlay_veneer Func_0810508c
	.global Func_080f8070
	.thumb_func
Func_080f8070:
	overlay_veneer Func_081051a8
	.global Func_080f8078
	.thumb_func
Func_080f8078:
	overlay_veneer Func_0810526c
	.global Func_080f8080
	.thumb_func
Func_080f8080:
	overlay_veneer Func_081050b8
	.global Func_080f8088
	.thumb_func
Func_080f8088:
	overlay_veneer Func_081052ac
