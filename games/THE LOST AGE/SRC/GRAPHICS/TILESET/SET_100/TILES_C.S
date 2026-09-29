@ Tileset_Set100TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set100TilesC
Tileset_Set100TilesC:
	.incbin "GRAPHICS/TILESET/SET_100/TILES_C.4bpp.mtf"
