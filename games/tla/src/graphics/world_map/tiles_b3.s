	.section .rodata
	.global WorldMap_TilesB3
WorldMap_TilesB3:
	.incbin "games/tla/src/graphics/world_map/tiles_b3.8bpp.lz"
	.byte 0x1c	@ PACKER: left in the buffer by WorldMap_TilesB2+0xa07
