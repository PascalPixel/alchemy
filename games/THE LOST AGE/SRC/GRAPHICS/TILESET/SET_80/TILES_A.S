@ Tileset_Set80TilesA: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set80TilesA
Tileset_Set80TilesA:
	.incbin "GRAPHICS/TILESET/SET_80/TILES_A.4bpp.mtf"
