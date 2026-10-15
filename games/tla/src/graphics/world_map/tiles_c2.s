	.section .rodata
	.global WorldMap_TilesC2
WorldMap_TilesC2:
	.incbin "games/tla/src/graphics/world_map/tiles_c2.8bpp.lz"
	.byte 0xc8, 0x74	@ PACKER: left in the buffer by WorldMap_TilesC1+0x146e
