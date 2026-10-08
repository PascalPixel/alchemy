	.section .rodata
	.global WorldMap_TilesJ
WorldMap_TilesJ:
	.incbin "graphics/world_map/tiles_j.8bpp.lz"
	.byte 0x52	@ PACKER: left in the buffer by WorldMap_TilesF+0x153f
