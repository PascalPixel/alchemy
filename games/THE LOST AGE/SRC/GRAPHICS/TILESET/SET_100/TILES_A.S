@ Tileset_Set100TilesA: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set100TilesA
Tileset_Set100TilesA:
	.incbin "GRAPHICS/TILESET/SET_100/TILES_A.4bpp.mtf"
