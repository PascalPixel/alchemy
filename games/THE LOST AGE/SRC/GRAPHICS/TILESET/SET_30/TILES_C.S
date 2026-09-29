@ Tileset_Set30TilesC: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set30TilesC
Tileset_Set30TilesC:
	.incbin "GRAPHICS/TILESET/SET_30/TILES_C.4bpp.mtf"
