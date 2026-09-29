@ WorldMap_TilesC4: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesC4
WorldMap_TilesC4:
	.incbin "GRAPHICS/WORLD_MAP/TILES_C4.8bpp.lz"
