@ WorldMap_TilesD6: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesD6
WorldMap_TilesD6:
	.incbin "GRAPHICS/WORLD_MAP/TILES_D6.8bpp.lz"
