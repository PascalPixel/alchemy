@ Tileset_Set01TilesA: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set01TilesA
Tileset_Set01TilesA:
	.incbin "GRAPHICS/TILESET/SET_01/TILES_A.4bpp.mtf"
