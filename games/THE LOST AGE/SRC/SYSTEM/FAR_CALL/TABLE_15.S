.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080381c0
	.thumb_func
Func_080381c0:
	overlay_veneer Func_0804bc08
	.global UiText_ShowMessageAndWaitCoreFar
	.thumb_func
UiText_ShowMessageAndWaitCoreFar:
	overlay_veneer Func_0803a5e0
	.global Func_080381d0
	.thumb_func
Func_080381d0:
	overlay_veneer Func_0803a54c
	.global Func_080381d8
	.thumb_func
Func_080381d8:
	overlay_veneer Func_0803f620
