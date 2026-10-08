	.section .rodata
	.global Tileset_Set36TilesB
Tileset_Set36TilesB:
	.incbin "GRAPHICS/TILESET/SET_36/TILES_B.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
