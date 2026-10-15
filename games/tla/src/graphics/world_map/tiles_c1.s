	.section .rodata
	.global WorldMap_TilesC1
WorldMap_TilesC1:
	.incbin "games/tla/src/graphics/world_map/tiles_c1.8bpp.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
