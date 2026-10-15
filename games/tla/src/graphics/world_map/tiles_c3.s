	.section .rodata
	.global WorldMap_TilesC3
WorldMap_TilesC3:
	.incbin "games/tla/src/graphics/world_map/tiles_c3.8bpp.lz"
	.byte 0x05, 0xf0	@ PACKER: left in the buffer by WorldMap_TilesC2+0x8f2
