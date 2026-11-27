	.section .rodata
	.4byte 0
	.global PointingHandTiles
PointingHandTiles:
	.incbin "games/tbs/src/graphics/ui/pointing_hand.4bpp16x16"
	.global DjinnMenuArrowUpTiles
DjinnMenuArrowUpTiles:
	.incbin "games/tbs/src/graphics/ui/djinn_arrow_up.4bpp16x16"
	.global DjinnMenuArrowDownTiles
DjinnMenuArrowDownTiles:
	.incbin "games/tbs/src/graphics/ui/djinn_arrow_down.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/glyph_bar.4bpp"
	.global NameEntryCaretTiles
NameEntryCaretTiles:
	.incbin "games/tbs/src/graphics/ui/name_entry_caret.4bpp16x16"
