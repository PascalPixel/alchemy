	.section .rodata
	.global WorldMap_TilesA
WorldMap_TilesA:
	.incbin "graphics/world_map/tiles_a.8bpp.lz"
	.byte 0x91, 0xb3	@ PACKER: left in the buffer by Field_DefaultMapCells+0x16ba
