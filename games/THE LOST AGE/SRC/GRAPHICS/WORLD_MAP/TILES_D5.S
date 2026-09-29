@ WorldMap_TilesD5: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesD5
WorldMap_TilesD5:
	.incbin "GRAPHICS/WORLD_MAP/TILES_D5.8bpp.lz"
