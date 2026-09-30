@ Far-call stub table at 0x08038248: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08038248
	.thumb_func
Func_08038248:
	overlay_veneer Func_0803d92c
	overlay_veneer Func_0803aae4
	overlay_veneer Func_08042588
	.global RenderOutput_RedrawSavedRectFar
	.thumb_func
RenderOutput_RedrawSavedRectFar:
	overlay_veneer RenderOutput_RedrawSavedRect
	.global Func_08038268
	.thumb_func
Func_08038268:
	overlay_veneer RenderOutput_ClearList
	.global UiWindow_SetTilemapEntryFar
	.thumb_func
UiWindow_SetTilemapEntryFar:
	overlay_veneer Func_0803c378
	overlay_veneer Func_0804db74
	overlay_veneer Menu_RunSelectionWithCursorObject
	overlay_veneer Func_0803f234
	overlay_veneer Func_08041b68
	overlay_veneer ItemIcon_Compose
