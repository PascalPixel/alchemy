	.section .rodata
	.4byte 0
	.global PointingHandTiles
PointingHandTiles:
	.incbin "games/tbs/src/graphics/ui/pointing_hand.4bpp16x16"
	.global DjinnMenu_ArrowUpTiles
DjinnMenu_ArrowUpTiles:
	.incbin "games/tbs/src/graphics/ui/djinn_arrow_up.4bpp16x16"
	.global DjinnMenu_ArrowDownTiles
DjinnMenu_ArrowDownTiles:
	.incbin "games/tbs/src/graphics/ui/djinn_arrow_down.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/glyph_bar.4bpp"
	.global NameEntry_CaretTiles
NameEntry_CaretTiles:
	.incbin "games/tbs/src/graphics/ui/name_entry_caret.4bpp16x16"
