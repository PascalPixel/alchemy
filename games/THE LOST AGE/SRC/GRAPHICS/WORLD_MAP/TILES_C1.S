@ WorldMap_TilesC1: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesC1
WorldMap_TilesC1:
	.incbin "GRAPHICS/WORLD_MAP/TILES_C1.8bpp.lz"
