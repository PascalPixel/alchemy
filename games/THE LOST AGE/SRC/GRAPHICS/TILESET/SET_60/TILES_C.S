@ Tileset_Set60TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set60TilesC
Tileset_Set60TilesC:
	.incbin "GRAPHICS/TILESET/SET_60/TILES_C.4bpp.mtf"
