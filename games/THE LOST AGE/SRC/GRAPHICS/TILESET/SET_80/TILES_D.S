@ Tileset_Set80TilesD: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set80TilesD
Tileset_Set80TilesD:
	.incbin "GRAPHICS/TILESET/SET_80/TILES_D.4bpp.mtf"
