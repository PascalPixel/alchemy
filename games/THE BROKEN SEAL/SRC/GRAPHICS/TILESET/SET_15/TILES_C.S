@ Tileset_Set15TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set15TilesC
Tileset_Set15TilesC:
	.incbin "GRAPHICS/TILESET/SET_15/TILES_C.4bpp.mtf"
