@ Display palette banks follow map 170: 4 tiles have observed uses.
@ Other tiles retain row 0; their display palette is unresolved in these maps.
@ Scene variants can use other palettes. Only each pixel's low nibble is encoded.
@ Tileset_Set67TilesB: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set67TilesB
Tileset_Set67TilesB:
	.incbin "GRAPHICS/TILESET/SET_67/TILES_B.4bpp.mtf"
