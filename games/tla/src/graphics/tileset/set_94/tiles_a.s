	.section .rodata
	.global Tileset_Set94TilesA
Tileset_Set94TilesA:
	.incbin "games/tla/src/graphics/tileset/set_94/tiles_a.4bpp.mtf"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
