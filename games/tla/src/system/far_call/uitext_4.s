.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
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
	.global UiWork_SetAltFlagAndClearTableFar
	.thumb_func
UiWork_SetAltFlagAndClearTableFar:
	overlay_veneer UiWork_SetAltFlagAndClearTable
	.global Menu_AnimateSelectionToEntryFar
	.thumb_func
Menu_AnimateSelectionToEntryFar:
	overlay_veneer Menu_AnimateSelectionToEntry
