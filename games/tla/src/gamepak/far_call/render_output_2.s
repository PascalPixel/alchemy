.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_08038248
	.thumb_func
Func_08038248:
	overlay_veneer Icon_DrawItem
	.global Text_DrawWideStringAtOffsetFar
	.thumb_func
Text_DrawWideStringAtOffsetFar:
	overlay_veneer Text_DrawWideStringAtOffset
	.global Window_FillFromSceneFar
	.thumb_func
Window_FillFromSceneFar:
	overlay_veneer Window_FillFromScene
	.global WindowSprite_RedrawSavedRectFar
	.thumb_func
WindowSprite_RedrawSavedRectFar:
	overlay_veneer WindowSprite_RedrawSavedRect
	.global WindowSprite_ClearListFar
	.thumb_func
WindowSprite_ClearListFar:
	overlay_veneer WindowSprite_ClearList
	.global Window_SetTilemapEntryFar
	.thumb_func
Window_SetTilemapEntryFar:
	overlay_veneer Window_SetTilemapEntry
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
