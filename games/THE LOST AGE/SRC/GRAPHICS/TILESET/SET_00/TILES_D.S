@ Tileset_Set00TilesD: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set00TilesD
Tileset_Set00TilesD:
	.incbin "GRAPHICS/TILESET/SET_00/TILES_D.4bpp.mtf"
