@ WorldMap_TilesI: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesI
WorldMap_TilesI:
	.incbin "GRAPHICS/WORLD_MAP/TILES_I.8bpp.lz"
