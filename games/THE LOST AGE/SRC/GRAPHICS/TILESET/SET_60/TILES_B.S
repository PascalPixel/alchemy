@ Display palette banks follow map 123: 68 tiles have observed uses.
@ Other tiles retain row 0; their display palette is unresolved in these maps.
@ Scene variants can use other palettes. Only each pixel's low nibble is encoded.
@ Tileset_Set60TilesB: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set60TilesB
Tileset_Set60TilesB:
	.incbin "GRAPHICS/TILESET/SET_60/TILES_B.4bpp.mtf"
