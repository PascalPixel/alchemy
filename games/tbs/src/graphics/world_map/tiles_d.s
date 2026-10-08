	.section .rodata
	.global WorldMap_TilesD
WorldMap_TilesD:
	.incbin "graphics/world_map/tiles_d.8bpp.lz"
	.byte 0x12, 0x9f	@ PACKER: left in the buffer by WorldMap_TilesC+0xb72
