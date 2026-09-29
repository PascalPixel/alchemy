@ Tileset_Set40TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set40TilesC
Tileset_Set40TilesC:
	.incbin "GRAPHICS/TILESET/SET_40/TILES_C.4bpp.mtf"
