	.section .rodata
	.global Tileset_Set36TilesB
Tileset_Set36TilesB:
	.incbin "graphics/tileset/set_36/tiles_b.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
