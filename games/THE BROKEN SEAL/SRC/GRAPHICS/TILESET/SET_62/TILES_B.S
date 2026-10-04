@ Display palette banks follow map 129: 57 tiles have observed uses.
@ Tile 215 uses rows 3 and 6 equally; this preview shows row 3.
@ Other tiles retain row 0; their display palette is unresolved in these maps.
@ Scene variants can use other palettes. Only each pixel's low nibble is encoded.
@ Tileset_Set62TilesB: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set62TilesB
Tileset_Set62TilesB:
	.incbin "GRAPHICS/TILESET/SET_62/TILES_B.4bpp.mtf"
