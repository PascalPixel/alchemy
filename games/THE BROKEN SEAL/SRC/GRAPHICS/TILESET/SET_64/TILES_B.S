@ Tileset_Set64TilesB: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set64TilesB
Tileset_Set64TilesB:
	.incbin "GRAPHICS/TILESET/SET_64/TILES_B.4bpp.mtf"
