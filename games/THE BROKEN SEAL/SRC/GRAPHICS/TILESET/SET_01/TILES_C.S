@ Tileset_Set01TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set01TilesC
Tileset_Set01TilesC:
	.incbin "GRAPHICS/TILESET/SET_01/TILES_C.4bpp.mtf"
