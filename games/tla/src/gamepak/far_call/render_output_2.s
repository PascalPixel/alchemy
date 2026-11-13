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
	.global ScanlineWindow_FillSpansFar
	.thumb_func
ScanlineWindow_FillSpansFar:
	overlay_veneer ScanlineWindow_FillSpans
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
	.global DebugSceneSelect_RunFar
	.thumb_func
DebugSceneSelect_RunFar:
	overlay_veneer DebugSceneSelect_Run
	.global DebugFlagEditor_RunFar
	.thumb_func
DebugFlagEditor_RunFar:
	overlay_veneer DebugFlagEditor_Run
	.global Func_08038288
	.thumb_func
Func_08038288:
	overlay_veneer Resource_LoadByModeIntoSlot
	.global Func_08038290
	.thumb_func
Func_08038290:
	overlay_veneer Window_MarkVisibleTextTiles
	.global ItemIcon_ComposeFar
	.thumb_func
ItemIcon_ComposeFar:
	overlay_veneer ItemIcon_Compose
