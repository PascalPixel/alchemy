@ Tileset_Set32TilesD: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set32TilesD
Tileset_Set32TilesD:
	.incbin "GRAPHICS/TILESET/SET_32/TILES_D.4bpp.mtf"
