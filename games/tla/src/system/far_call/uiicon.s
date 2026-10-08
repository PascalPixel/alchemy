.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global UiIcon_CopyResourceToSlotFar
	.thumb_func
UiIcon_CopyResourceToSlotFar:
	overlay_veneer UiIcon_CopyResourceToSlot
	.global Func_08038418
	.thumb_func
Func_08038418:
	overlay_veneer Func_0803d020
	.global Func_08038420
	.thumb_func
Func_08038420:
	overlay_veneer Func_0803c068
	.global Func_08038428
	.thumb_func
Func_08038428:
	overlay_veneer Func_0803c170
	.global Func_08038430
	.thumb_func
Func_08038430:
	overlay_veneer Func_0803c198
