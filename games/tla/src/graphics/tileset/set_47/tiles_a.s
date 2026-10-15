	.section .rodata
	.global Tileset_Set47TilesA
Tileset_Set47TilesA:
	.incbin "games/tla/src/graphics/tileset/set_47/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
