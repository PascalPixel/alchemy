@ WorldMap_TilesA: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesA
WorldMap_TilesA:
	.incbin "GRAPHICS/WORLD_MAP/TILES_A.8bpp.lz"
