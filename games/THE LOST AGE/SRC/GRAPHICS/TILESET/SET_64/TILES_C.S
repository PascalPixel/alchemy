@ Tileset_Set64TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set64TilesC
Tileset_Set64TilesC:
	.incbin "GRAPHICS/TILESET/SET_64/TILES_C.4bpp.mtf"
