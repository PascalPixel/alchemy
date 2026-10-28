.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080381c0
	.thumb_func
Func_080381c0:
	overlay_veneer Func_0804bc08
	.global Message_ShowAndWaitCoreFar
	.thumb_func
Message_ShowAndWaitCoreFar:
	overlay_veneer Message_ShowAndWaitCore
	.global Func_080381d0
	.thumb_func
Func_080381d0:
	overlay_veneer Message_Show
	.global Func_080381d8
	.thumb_func
Func_080381d8:
	overlay_veneer Func_0803f620
