	.section .rodata
	.balign 2
@ 色見本と文字のタイル
	.global gDebug_PaletteSwatchTiles
gDebug_PaletteSwatchTiles:
	.incbin "games/common/src/debug/swatch.4bpp.hlz"
