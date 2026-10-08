	.section .rodata
	.global WorldMap_TilesE
WorldMap_TilesE:
	.incbin "graphics/world_map/tiles_e.8bpp.lz"
	.byte 0x48	@ PACKER: left in the buffer by Field_DefaultMapCells+0x1ea7
