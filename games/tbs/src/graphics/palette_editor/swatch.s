	.section .rodata
	.balign 2
@ Colour swatch and lettering tiles
	.global gDebug_PaletteSwatchTiles
gDebug_PaletteSwatchTiles:
	.incbin "games/tbs/src/graphics/palette_editor/swatch.4bpp.hlz"
