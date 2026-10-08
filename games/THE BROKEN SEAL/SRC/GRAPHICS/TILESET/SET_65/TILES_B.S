	.section .rodata
	.global Tileset_Set65TilesB
Tileset_Set65TilesB:
	.incbin "GRAPHICS/TILESET/SET_65/TILES_B.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
