	.section .rodata
	.global WorldMap_TilesG
WorldMap_TilesG:
	.incbin "graphics/world_map/tiles_g.8bpp.lz"
	.byte 0x20, 0x75	@ PACKER: left in the buffer by WorldMap_TilesF+0x14ae
