@ WorldMap_TilesB1: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesB1
WorldMap_TilesB1:
	.incbin "GRAPHICS/WORLD_MAP/TILES_B1.8bpp.lz"
