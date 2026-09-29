@ WorldMap_TilesF: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesF
WorldMap_TilesF:
	.incbin "GRAPHICS/WORLD_MAP/TILES_F.8bpp.lz"
