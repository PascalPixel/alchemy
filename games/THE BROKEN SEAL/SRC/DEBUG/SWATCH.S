	.section .rodata
	.balign 2
@ 色見本と文字のタイル
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "DEBUG/SWATCH.4bpp.hlz"
