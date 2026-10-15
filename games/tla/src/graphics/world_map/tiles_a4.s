	.section .rodata
	.global WorldMap_TilesA4
WorldMap_TilesA4:
	.incbin "games/tla/src/graphics/world_map/tiles_a4.8bpp.lz"
	.byte 0x23	@ PACKER: left in the buffer by WorldMap_TilesA2+0x1083
