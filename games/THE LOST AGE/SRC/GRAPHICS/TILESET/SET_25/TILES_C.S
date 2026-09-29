@ Tileset_Set25TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set25TilesC
Tileset_Set25TilesC:
	.incbin "GRAPHICS/TILESET/SET_25/TILES_C.4bpp.mtf"
