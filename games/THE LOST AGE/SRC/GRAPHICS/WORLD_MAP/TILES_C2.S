@ WorldMap_TilesC2: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesC2
WorldMap_TilesC2:
	.incbin "GRAPHICS/WORLD_MAP/TILES_C2.8bpp.lz"
