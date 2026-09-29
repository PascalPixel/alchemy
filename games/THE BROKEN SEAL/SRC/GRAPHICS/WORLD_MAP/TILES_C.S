@ WorldMap_TilesC: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesC
WorldMap_TilesC:
	.incbin "GRAPHICS/WORLD_MAP/TILES_C.8bpp.lz"
