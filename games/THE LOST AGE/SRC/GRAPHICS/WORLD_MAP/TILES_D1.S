@ WorldMap_TilesD1: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesD1
WorldMap_TilesD1:
	.incbin "GRAPHICS/WORLD_MAP/TILES_D1.8bpp.lz"
