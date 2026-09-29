@ Tileset_Set10TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set10TilesC
Tileset_Set10TilesC:
	.incbin "GRAPHICS/TILESET/SET_10/TILES_C.4bpp.mtf"
