@ Tileset_Set00TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set00TilesC
Tileset_Set00TilesC:
	.incbin "GRAPHICS/TILESET/SET_00/TILES_C.4bpp.mtf"
