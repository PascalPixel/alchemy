.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global YesNoMenu_RunAtFar
	.thumb_func
YesNoMenu_RunAtFar:
	overlay_veneer YesNoMenu_RunAt
	.global Resource_LoadFixedBlockBIntoFreeSlotFar
	.thumb_func
Resource_LoadFixedBlockBIntoFreeSlotFar:
	overlay_veneer Resource_LoadFixedBlockBIntoFreeSlot
	.global Text_MeasureMessageFar
	.thumb_func
Text_MeasureMessageFar:
	overlay_veneer Text_MeasureMessage
	.global Window_HighlightRectFar
	.thumb_func
Window_HighlightRectFar:
	overlay_veneer Window_HighlightRect
	.global Ability_LoadGlyphFar
	.thumb_func
Ability_LoadGlyphFar:
	overlay_veneer Ability_LoadGlyph
	.global Graphics_ExpandVramTilesByColorTableFar
	.thumb_func
Graphics_ExpandVramTilesByColorTableFar:
	overlay_veneer Graphics_ExpandVramTilesByColorTable
	.global Text_UseExtraTilesFar
	.thumb_func
Text_UseExtraTilesFar:
	overlay_veneer Text_UseExtraTiles
	.global Menu_AnimateSelectionToEntryFar
	.thumb_func
Menu_AnimateSelectionToEntryFar:
	overlay_veneer Menu_AnimateSelectionToEntry
