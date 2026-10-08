.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080381c0
	.thumb_func
Func_080381c0:
	overlay_veneer Func_0804bc08
	.global UiText_ShowMessageAndWaitCoreFar
	.thumb_func
UiText_ShowMessageAndWaitCoreFar:
	overlay_veneer UiMessage_ShowAndWait
	.global Func_080381d0
	.thumb_func
Func_080381d0:
	overlay_veneer UiMessage_Show
	.global Func_080381d8
	.thumb_func
Func_080381d8:
	overlay_veneer Func_0803f620
