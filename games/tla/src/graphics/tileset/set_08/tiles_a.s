	.section .rodata
	.global Tileset_Set08TilesA
Tileset_Set08TilesA:
	.incbin "games/tla/src/graphics/tileset/set_08/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
