	.section .rodata
	.global WorldMap_TilesD6
WorldMap_TilesD6:
	.incbin "games/tla/src/graphics/world_map/tiles_d6.8bpp.lz"
	.byte 0x2c, 0xa2	@ PACKER: left in the buffer by WorldMap_TilesD5+0x352
