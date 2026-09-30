@ Far-call stub table at 0x08038218: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08038218
	.thumb_func
Func_08038218:
	overlay_veneer Func_0803a530
	.global Func_08038220
	.thumb_func
Func_08038220:
	overlay_veneer Func_080396dc
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
