@ WorldMap_TilesA6: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesA6
WorldMap_TilesA6:
	.incbin "GRAPHICS/WORLD_MAP/TILES_A6.8bpp.lz"
