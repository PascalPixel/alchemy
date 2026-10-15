	.section .rodata
	.global WorldMap_TilesC6
WorldMap_TilesC6:
	.incbin "games/tla/src/graphics/world_map/tiles_c6.8bpp.lz"
	.byte 0x21	@ PACKER: left in the buffer by WorldMap_TilesC5+0xbeb
