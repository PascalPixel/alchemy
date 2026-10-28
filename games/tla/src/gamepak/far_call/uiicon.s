.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Icon_CopyResourceToSlotFar
	.thumb_func
Icon_CopyResourceToSlotFar:
	overlay_veneer Icon_CopyResourceToSlot
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
