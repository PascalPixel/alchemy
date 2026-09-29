@ Tileset_Set80TilesB: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set80TilesB
Tileset_Set80TilesB:
	.incbin "GRAPHICS/TILESET/SET_80/TILES_B.4bpp.mtf"
