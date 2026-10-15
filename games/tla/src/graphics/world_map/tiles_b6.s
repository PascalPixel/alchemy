	.section .rodata
	.global WorldMap_TilesB6
WorldMap_TilesB6:
	.incbin "games/tla/src/graphics/world_map/tiles_b6.8bpp.lz"
	.byte 0x0b, 0x4c	@ PACKER: left in the buffer by WorldMap_TilesB5+0xf92
