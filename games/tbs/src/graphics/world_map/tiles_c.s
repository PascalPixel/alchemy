	.section .rodata
	.global WorldMap_TilesC
WorldMap_TilesC:
	.incbin "graphics/world_map/tiles_c.8bpp.lz"
	.byte 0x63, 0xd3, 0xb6	@ PACKER: left in the buffer by WorldMap_TilesB+0xbc9
