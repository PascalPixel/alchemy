	.section .rodata
	.global Tileset_Set31TilesB
Tileset_Set31TilesB:
	.incbin "games/tla/src/graphics/tileset/set_31/tiles_b.4bpp.mtf"
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
