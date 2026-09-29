@ Tileset_Set100TilesD: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set100TilesD
Tileset_Set100TilesD:
	.incbin "GRAPHICS/TILESET/SET_100/TILES_D.4bpp.mtf"
