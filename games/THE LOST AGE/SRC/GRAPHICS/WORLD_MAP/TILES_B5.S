@ WorldMap_TilesB5: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesB5
WorldMap_TilesB5:
	.incbin "GRAPHICS/WORLD_MAP/TILES_B5.8bpp.lz"
