@ WorldMap_TilesD4: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesD4
WorldMap_TilesD4:
	.incbin "GRAPHICS/WORLD_MAP/TILES_D4.8bpp.lz"
