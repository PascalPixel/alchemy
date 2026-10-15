	.section .rodata
	.global Tileset_Set113TilesA
Tileset_Set113TilesA:
	.incbin "games/tla/src/graphics/tileset/set_113/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
