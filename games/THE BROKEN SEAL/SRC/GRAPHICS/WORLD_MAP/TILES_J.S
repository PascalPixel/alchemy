@ WorldMap_TilesJ: 256 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesJ
WorldMap_TilesJ:
	.incbin "GRAPHICS/WORLD_MAP/TILES_J.8bpp.lz"
