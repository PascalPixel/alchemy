.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Scene_GetAreaNameIndexFar
	.thumb_func
Scene_GetAreaNameIndexFar:
	overlay_veneer Scene_GetAreaNameIndex
	.global Func_080c8650
	.thumb_func
Func_080c8650:
	overlay_veneer Func_080d1ee8
	.global Func_080c8658
	.thumb_func
Func_080c8658:
	overlay_veneer Func_080d1f0c
	.global Func_080c8660
	.thumb_func
Func_080c8660:
	overlay_veneer Func_080d1f18
	.global Func_080c8668
	.thumb_func
Func_080c8668:
	overlay_veneer Func_080d1f20
	.global Func_080c8670
	.thumb_func
Func_080c8670:
	overlay_veneer Event_WaitTwoSecondsOrPress
	.global Func_080c8678
	.thumb_func
Func_080c8678:
	overlay_veneer Func_080d20fc
	.global Actor_SetActionCallbackFar
	.thumb_func
Actor_SetActionCallbackFar:
	overlay_veneer Actor_SetActionCallback
	.global Func_080c8688
	.thumb_func
Func_080c8688:
	overlay_veneer Func_080d2c64
	.global Event_NoOpFar
	.thumb_func
Event_NoOpFar:
	overlay_veneer Event_NoOp
