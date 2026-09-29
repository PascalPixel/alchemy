@ Tileset_Set80TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set80TilesC
Tileset_Set80TilesC:
	.incbin "GRAPHICS/TILESET/SET_80/TILES_C.4bpp.mtf"
