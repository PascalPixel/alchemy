	.section .rodata
	.balign 2
@ 色見本と文字のタイル
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "games/common/src/debug/swatch.4bpp.hlz"
