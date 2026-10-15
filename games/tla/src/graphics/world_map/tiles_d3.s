	.section .rodata
	.global WorldMap_TilesD3
WorldMap_TilesD3:
	.incbin "games/tla/src/graphics/world_map/tiles_d3.8bpp.lz"
	.byte 0xe6, 0xf4, 0x80	@ PACKER: left in the buffer by WorldMap_TilesD2+0xf3d
