	.section .rodata
	.4byte 0
	.global gPointingHandTiles
gPointingHandTiles:
	.incbin "games/tbs/src/graphics/ui/pointing_hand.4bpp16x16"
	.global gDjinnMenu_ArrowUpTiles
gDjinnMenu_ArrowUpTiles:
	.incbin "games/tbs/src/graphics/ui/djinn_arrow_up.4bpp16x16"
	.global gDjinnMenu_ArrowDownTiles
gDjinnMenu_ArrowDownTiles:
	.incbin "games/tbs/src/graphics/ui/djinn_arrow_down.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/glyph_bar.4bpp"
	.global gNameEntry_CaretTiles
gNameEntry_CaretTiles:
	.incbin "games/tbs/src/graphics/ui/name_entry_caret.4bpp16x16"
