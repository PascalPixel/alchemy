.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_08038248
	.thumb_func
Func_08038248:
	overlay_veneer Func_0803d92c
	.global UiText_RenderWideStringAtOffsetFar
	.thumb_func
UiText_RenderWideStringAtOffsetFar:
	overlay_veneer UiText_RenderWideStringAtOffset
	.global UiWindow_FillFromSceneFar
	.thumb_func
UiWindow_FillFromSceneFar:
	overlay_veneer UiWindow_FillFromScene
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
	overlay_veneer UiWindow_SetTilemapEntry
	.global Menu_RunSelectionFar
	.thumb_func
Menu_RunSelectionFar:
	overlay_veneer Menu_RunSelection
	.global Menu_RunSelectionWithCursorObjectFar
	.thumb_func
Menu_RunSelectionWithCursorObjectFar:
	overlay_veneer Menu_RunSelectionWithCursorObject
	.global Func_08038288
	.thumb_func
Func_08038288:
	overlay_veneer Resource_LoadByModeIntoSlot
	.global Func_08038290
	.thumb_func
Func_08038290:
	overlay_veneer Func_08041b68
	.global ItemIcon_ComposeFar
	.thumb_func
ItemIcon_ComposeFar:
	overlay_veneer ItemIcon_Compose
