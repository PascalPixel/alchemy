	.section .rodata
	.global WorldMap_TilesD1
WorldMap_TilesD1:
	.incbin "games/tla/src/graphics/world_map/tiles_d1.8bpp.lz"
	.byte 0xc2, 0x44	@ PACKER: left in the buffer by WorldMap_TilesC6+0x896
