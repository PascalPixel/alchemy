.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Ui_GetTableWordZeroFar
	.thumb_func
Ui_GetTableWordZeroFar:
	overlay_veneer Ui_GetTableWordZero
	.global Func_080383d8
	.thumb_func
Func_080383d8:
	overlay_veneer Func_0803a60c
	.global Func_080383e0
	.thumb_func
Func_080383e0:
	overlay_veneer Func_0804325c
