@ WorldMap_TilesE: 256 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesE
WorldMap_TilesE:
	.incbin "GRAPHICS/WORLD_MAP/TILES_E.8bpp.lz"
