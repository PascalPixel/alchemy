	.section .rodata
	.global Tileset_Set31TilesB
Tileset_Set31TilesB:
	.incbin "games/tla/src/graphics/tileset/set_31/tiles_b.4bpp.mtf"
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
