@ Tileset_Set90TilesD: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set90TilesD
Tileset_Set90TilesD:
	.incbin "GRAPHICS/TILESET/SET_90/TILES_D.4bpp.mtf"
