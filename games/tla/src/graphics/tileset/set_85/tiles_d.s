	.section .rodata
	.global Tileset_Set85TilesD
Tileset_Set85TilesD:
	.incbin "games/tla/src/graphics/tileset/set_85/tiles_d.4bpp.mtf"
	.ifndef TLA_EDITION_JA
	.ifndef TLA_EDITION_DE
	.ifndef TLA_EDITION_ES
	.ifndef TLA_EDITION_FR
	.ifndef TLA_EDITION_IT
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
	.endif
	.endif
	.endif
