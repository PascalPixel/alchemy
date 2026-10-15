	.section .rodata
	.balign 2
@ Colour swatch and lettering tiles
	.global gDebug_PaletteSwatchTiles
gDebug_PaletteSwatchTiles:
	.incbin "games/common/src/debug/swatch.4bpp.hlz"
