@ WorldMap_TilesC5: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesC5
WorldMap_TilesC5:
	.incbin "GRAPHICS/WORLD_MAP/TILES_C5.8bpp.lz"
