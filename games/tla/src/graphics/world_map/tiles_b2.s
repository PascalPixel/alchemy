	.section .rodata
	.global WorldMap_TilesB2
WorldMap_TilesB2:
	.incbin "games/tla/src/graphics/world_map/tiles_b2.8bpp.lz"
	.byte 0x00	@ PACKER: left in the buffer by WorldMap_TilesB1+0x10c3
