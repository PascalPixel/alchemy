	.section .rodata
	.global Tileset_Set65TilesB
Tileset_Set65TilesB:
	.incbin "graphics/tileset/set_65/tiles_b.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
