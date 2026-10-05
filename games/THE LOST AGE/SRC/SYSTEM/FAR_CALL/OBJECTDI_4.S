.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global ObjectDispatch_InitFromTable4WithArgumentFar
	.thumb_func
ObjectDispatch_InitFromTable4WithArgumentFar:
	overlay_veneer ObjectDispatch_InitFromTable4WithArgument
	.global Func_080200e0
	.thumb_func
Func_080200e0:
	overlay_veneer Func_08023510
