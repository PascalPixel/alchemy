@ WorldMap_TilesD: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesD
WorldMap_TilesD:
	.incbin "GRAPHICS/WORLD_MAP/TILES_D.8bpp.lz"
