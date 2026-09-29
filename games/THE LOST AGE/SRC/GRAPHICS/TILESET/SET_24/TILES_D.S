@ Tileset_Set24TilesD: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set24TilesD
Tileset_Set24TilesD:
	.incbin "GRAPHICS/TILESET/SET_24/TILES_D.4bpp.mtf"
