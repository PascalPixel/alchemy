@ WorldMap_TilesA3: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesA3
WorldMap_TilesA3:
	.incbin "GRAPHICS/WORLD_MAP/TILES_A3.8bpp.lz"
