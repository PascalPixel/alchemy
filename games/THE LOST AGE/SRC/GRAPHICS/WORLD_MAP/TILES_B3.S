@ WorldMap_TilesB3: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesB3
WorldMap_TilesB3:
	.incbin "GRAPHICS/WORLD_MAP/TILES_B3.8bpp.lz"
