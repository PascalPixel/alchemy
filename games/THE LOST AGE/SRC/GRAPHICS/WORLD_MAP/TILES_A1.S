@ WorldMap_TilesA1: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesA1
WorldMap_TilesA1:
	.incbin "GRAPHICS/WORLD_MAP/TILES_A1.8bpp.lz"
