@ Tileset_Set96TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set96TilesC
Tileset_Set96TilesC:
	.incbin "GRAPHICS/TILESET/SET_96/TILES_C.4bpp.mtf"
