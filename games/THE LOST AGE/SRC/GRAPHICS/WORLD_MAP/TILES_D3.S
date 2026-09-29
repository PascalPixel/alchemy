@ WorldMap_TilesD3: 128 tiles of 8-bit colour with the set's palette, packed by the
@ resource packer.
	.section .rodata
	.global WorldMap_TilesD3
WorldMap_TilesD3:
	.incbin "GRAPHICS/WORLD_MAP/TILES_D3.8bpp.lz"
