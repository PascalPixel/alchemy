	.section .rodata
	.global WorldMap_TilesC4
WorldMap_TilesC4:
	.incbin "games/tla/src/graphics/world_map/tiles_c4.8bpp.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
