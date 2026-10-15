	.section .rodata
	.global WorldMap_TilesA1
WorldMap_TilesA1:
	.incbin "games/tla/src/graphics/world_map/tiles_a1.8bpp.lz"
	.byte 0x23, 0xe0	@ PACKER: left in the buffer by Field_DefaultMapCells+0x176a
