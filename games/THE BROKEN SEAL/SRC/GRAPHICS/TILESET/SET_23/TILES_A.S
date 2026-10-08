	.section .rodata
	.global Tileset_Set23TilesA
Tileset_Set23TilesA:
	.incbin "GRAPHICS/TILESET/SET_23/TILES_A.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
