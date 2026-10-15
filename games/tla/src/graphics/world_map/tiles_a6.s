	.section .rodata
	.global WorldMap_TilesA6
WorldMap_TilesA6:
	.incbin "games/tla/src/graphics/world_map/tiles_a6.8bpp.lz"
	.byte 0x4c	@ PACKER: left in the buffer by WorldMap_TilesA5+0xf3f
