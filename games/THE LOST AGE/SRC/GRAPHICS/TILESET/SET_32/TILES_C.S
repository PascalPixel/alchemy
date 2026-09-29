@ Tileset_Set32TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set32TilesC
Tileset_Set32TilesC:
	.incbin "GRAPHICS/TILESET/SET_32/TILES_C.4bpp.mtf"
