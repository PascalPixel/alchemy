.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Menu_RunConfirmSelectionAtFar
	.thumb_func
Menu_RunConfirmSelectionAtFar:
	overlay_veneer Menu_RunConfirmSelectionAt
	.global Resource_LoadFixedBlockBIntoFreeSlotFar
	.thumb_func
Resource_LoadFixedBlockBIntoFreeSlotFar:
	overlay_veneer Resource_LoadFixedBlockBIntoFreeSlot
	.global UiText_MeasureResourceEntriesFar
	.thumb_func
UiText_MeasureResourceEntriesFar:
	overlay_veneer UiText_MeasureResourceEntries
	.global Ui_SetRectHighlightFar
	.thumb_func
Ui_SetRectHighlightFar:
	overlay_veneer Ui_SetRectHighlight
	.global Ability_LoadGlyphFar
	.thumb_func
Ability_LoadGlyphFar:
	overlay_veneer Ability_LoadGlyph
	.global Graphics_ExpandVramTilesByColorTableFar
	.thumb_func
Graphics_ExpandVramTilesByColorTableFar:
	overlay_veneer Graphics_ExpandVramTilesByColorTable
	.global Func_080383c0
	.thumb_func
Func_080383c0:
	overlay_veneer Func_08041c0c
	.global Menu_AnimateSelectionToEntryFar
	.thumb_func
Menu_AnimateSelectionToEntryFar:
	overlay_veneer Menu_AnimateSelectionToEntry
