	.section .rodata
	.global WorldMap_TilesC5
WorldMap_TilesC5:
	.incbin "games/tla/src/graphics/world_map/tiles_c5.8bpp.lz"
	.byte 0xec, 0xab	@ PACKER: left in the buffer by WorldMap_TilesC2+0xeee
