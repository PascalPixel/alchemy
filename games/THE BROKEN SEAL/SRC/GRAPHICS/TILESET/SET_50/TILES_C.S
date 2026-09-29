@ Tileset_Set50TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set50TilesC
Tileset_Set50TilesC:
	.incbin "GRAPHICS/TILESET/SET_50/TILES_C.4bpp.mtf"
