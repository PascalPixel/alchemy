@ WorldMap_TilesC6: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesC6
WorldMap_TilesC6:
	.incbin "GRAPHICS/WORLD_MAP/TILES_C6.8bpp.lz"
