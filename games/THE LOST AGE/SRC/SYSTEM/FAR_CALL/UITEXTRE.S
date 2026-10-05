.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global UiWork_FinalizeSharedSlotFar
	.thumb_func
UiWork_FinalizeSharedSlotFar:
	overlay_veneer Func_0803a530
	.global Func_08038220
	.thumb_func
Func_08038220:
	overlay_veneer UiWork_ResetFreeChannel
	.global UiTextResource_InitializeFar
	.thumb_func
UiTextResource_InitializeFar:
	overlay_veneer UiTextResource_Initialize
	.global UiTextResource_SetPositionFar
	.thumb_func
UiTextResource_SetPositionFar:
	overlay_veneer UiTextResource_SetPosition
	.global UiTextResource_ReleaseFar
	.thumb_func
UiTextResource_ReleaseFar:
	overlay_veneer UiTextResource_Release
	.global Func_08038240
	.thumb_func
Func_08038240:
	overlay_veneer Func_0803f968
