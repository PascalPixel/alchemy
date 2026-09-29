@ Tileset_Set64TilesA: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set64TilesA
Tileset_Set64TilesA:
	.incbin "GRAPHICS/TILESET/SET_64/TILES_A.4bpp.mtf"
