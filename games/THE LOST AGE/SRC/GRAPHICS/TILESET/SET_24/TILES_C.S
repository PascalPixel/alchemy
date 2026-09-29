@ Tileset_Set24TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set24TilesC
Tileset_Set24TilesC:
	.incbin "GRAPHICS/TILESET/SET_24/TILES_C.4bpp.mtf"
