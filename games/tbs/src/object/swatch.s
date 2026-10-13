	.section .rodata
	.balign 2
@ 色見本と文字のタイル
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "object/swatch.4bpp.hlz"
