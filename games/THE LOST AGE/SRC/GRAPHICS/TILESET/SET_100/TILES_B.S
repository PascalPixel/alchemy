@ Tileset_Set100TilesB: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set100TilesB
Tileset_Set100TilesB:
	.incbin "GRAPHICS/TILESET/SET_100/TILES_B.4bpp.mtf"
