	.section .rodata
	.global WorldMap_TilesD5
WorldMap_TilesD5:
	.incbin "games/tla/src/graphics/world_map/tiles_d5.8bpp.lz"
	.byte 0xc7	@ PACKER: left in the buffer by WorldMap_TilesD4+0x49f
