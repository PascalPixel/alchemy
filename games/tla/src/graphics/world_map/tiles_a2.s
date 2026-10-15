	.section .rodata
	.global WorldMap_TilesA2
WorldMap_TilesA2:
	.incbin "games/tla/src/graphics/world_map/tiles_a2.8bpp.lz"
	.byte 0xbe, 0x9c, 0x39	@ PACKER: left in the buffer by WorldMap_TilesA1+0x1535
