@ WorldMap_TilesB2: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesB2
WorldMap_TilesB2:
	.incbin "GRAPHICS/WORLD_MAP/TILES_B2.8bpp.lz"
