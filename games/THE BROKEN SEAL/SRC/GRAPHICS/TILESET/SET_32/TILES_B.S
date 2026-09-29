@ Tileset_Set32TilesB: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set32TilesB
Tileset_Set32TilesB:
	.incbin "GRAPHICS/TILESET/SET_32/TILES_B.4bpp.mtf"
