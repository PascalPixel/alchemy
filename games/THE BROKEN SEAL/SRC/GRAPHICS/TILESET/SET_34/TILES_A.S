	.section .rodata
	.global Tileset_Set34TilesA
Tileset_Set34TilesA:
	.incbin "GRAPHICS/TILESET/SET_34/TILES_A.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
