	.section .rodata
	.global Tileset_Set23TilesA
Tileset_Set23TilesA:
	.incbin "graphics/tileset/set_23/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
