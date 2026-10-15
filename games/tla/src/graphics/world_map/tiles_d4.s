	.section .rodata
	.global WorldMap_TilesD4
WorldMap_TilesD4:
	.incbin "games/tla/src/graphics/world_map/tiles_d4.8bpp.lz"
	.byte 0x84	@ PACKER: left in the buffer by WorldMap_TilesD3+0x5cf
