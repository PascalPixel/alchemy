@ Tileset_Set12TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set12TilesC
Tileset_Set12TilesC:
	.incbin "GRAPHICS/TILESET/SET_12/TILES_C.4bpp.mtf"
