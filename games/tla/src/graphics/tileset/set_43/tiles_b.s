	.section .rodata
	.global Tileset_Set43TilesB
Tileset_Set43TilesB:
	.incbin "games/tla/src/graphics/tileset/set_43/tiles_b.4bpp.mtf"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
