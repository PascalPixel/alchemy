.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Window_CloseSharedFar
	.thumb_func
Window_CloseSharedFar:
	overlay_veneer Message_Close
	.global Message_ResetIdleFar
	.thumb_func
Message_ResetIdleFar:
	overlay_veneer Message_ResetIdle
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
