.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_08038358
	.thumb_func
Func_08038358:
	overlay_veneer Func_0804e0d0
	.global UiWindow_FillScreenBlockRectFar
	.thumb_func
UiWindow_FillScreenBlockRectFar:
	overlay_veneer UiWindow_FillScreenBlockRect
