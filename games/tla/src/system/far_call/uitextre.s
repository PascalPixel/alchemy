.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global UiWork_FinalizeSharedSlotFar
	.thumb_func
UiWork_FinalizeSharedSlotFar:
	overlay_veneer UiMessage_Close
	.global UiWork_ResetFreeChannelFar
	.thumb_func
UiWork_ResetFreeChannelFar:
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
