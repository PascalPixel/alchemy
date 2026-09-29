@ Tileset_Set16TilesA: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set16TilesA
Tileset_Set16TilesA:
	.incbin "GRAPHICS/TILESET/SET_16/TILES_A.4bpp.mtf"
