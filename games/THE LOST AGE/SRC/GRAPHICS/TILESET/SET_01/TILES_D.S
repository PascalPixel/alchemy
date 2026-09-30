@ Tileset_Set01TilesD: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set01TilesD
Tileset_Set01TilesD:
	.incbin "COMMON/GRAPHICS/TILESET/SET_07/TILES_D.4bpp.mtf"
