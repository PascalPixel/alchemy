@ WorldMap_TilesD2: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesD2
WorldMap_TilesD2:
	.incbin "GRAPHICS/WORLD_MAP/TILES_D2.8bpp.lz"
