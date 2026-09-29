@ WorldMap_TilesG: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesG
WorldMap_TilesG:
	.incbin "GRAPHICS/WORLD_MAP/TILES_G.8bpp.lz"
