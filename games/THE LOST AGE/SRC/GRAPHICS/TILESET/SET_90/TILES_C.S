@ Tileset_Set90TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set90TilesC
Tileset_Set90TilesC:
	.incbin "GRAPHICS/TILESET/SET_90/TILES_C.4bpp.mtf"
