@ WorldMap_TilesC3: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesC3
WorldMap_TilesC3:
	.incbin "GRAPHICS/WORLD_MAP/TILES_C3.8bpp.lz"
