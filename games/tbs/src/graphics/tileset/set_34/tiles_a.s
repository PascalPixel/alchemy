	.section .rodata
	.global Tileset_Set34TilesA
Tileset_Set34TilesA:
	.incbin "graphics/tileset/set_34/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
