@ WorldMap_TilesB: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesB
WorldMap_TilesB:
	.incbin "GRAPHICS/WORLD_MAP/TILES_B.8bpp.lz"
