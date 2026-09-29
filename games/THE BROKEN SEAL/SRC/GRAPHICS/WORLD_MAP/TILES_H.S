@ WorldMap_TilesH: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesH
WorldMap_TilesH:
	.incbin "GRAPHICS/WORLD_MAP/TILES_H.8bpp.lz"
