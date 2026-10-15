	.section .rodata
	.global Tileset_Set85TilesD
Tileset_Set85TilesD:
	.incbin "games/tla/src/graphics/tileset/set_85/tiles_d.4bpp.mtf"
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
