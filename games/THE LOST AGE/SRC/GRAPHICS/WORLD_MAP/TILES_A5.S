@ WorldMap_TilesA5: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesA5
WorldMap_TilesA5:
	.incbin "GRAPHICS/WORLD_MAP/TILES_A5.8bpp.lz"
