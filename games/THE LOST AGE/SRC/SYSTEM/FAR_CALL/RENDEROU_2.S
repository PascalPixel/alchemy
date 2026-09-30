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
	.global Func_08038250
	.thumb_func
Func_08038250:
	overlay_veneer Func_0803aae4
	.global Func_08038258
	.thumb_func
Func_08038258:
	overlay_veneer Func_08042588
	.global RenderOutput_RedrawSavedRectFar
	.thumb_func
RenderOutput_RedrawSavedRectFar:
	overlay_veneer RenderOutput_RedrawSavedRect
	.global RenderOutput_ClearListFar
	.thumb_func
RenderOutput_ClearListFar:
	overlay_veneer RenderOutput_ClearList
	.global UiWindow_SetTilemapEntryFar
	.thumb_func
UiWindow_SetTilemapEntryFar:
	overlay_veneer Func_0803c378
	.global Func_08038278
	.thumb_func
Func_08038278:
	overlay_veneer Func_0804db74
	.global Menu_RunSelectionWithCursorObjectFar
	.thumb_func
Menu_RunSelectionWithCursorObjectFar:
	overlay_veneer Menu_RunSelectionWithCursorObject
	.global Func_08038288
	.thumb_func
Func_08038288:
	overlay_veneer Func_0803f234
	.global Func_08038290
	.thumb_func
Func_08038290:
	overlay_veneer Func_08041b68
	.global ItemIcon_ComposeFar
	.thumb_func
ItemIcon_ComposeFar:
	overlay_veneer ItemIcon_Compose
