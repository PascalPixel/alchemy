@ WorldMap_TilesB4: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesB4
WorldMap_TilesB4:
	.incbin "GRAPHICS/WORLD_MAP/TILES_B4.8bpp.lz"
