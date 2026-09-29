@ Tileset_Set48TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set48TilesC
Tileset_Set48TilesC:
	.incbin "GRAPHICS/TILESET/SET_48/TILES_C.4bpp.mtf"
