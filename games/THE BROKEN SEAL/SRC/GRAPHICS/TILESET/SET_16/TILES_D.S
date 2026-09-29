@ Tileset_Set16TilesD: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set16TilesD
Tileset_Set16TilesD:
	.incbin "GRAPHICS/TILESET/SET_16/TILES_D.4bpp.mtf"
