	.section .rodata
	.global Tileset_Set81TilesA
Tileset_Set81TilesA:
	.incbin "games/tla/src/graphics/tileset/set_81/tiles_a.4bpp.mtf"
	.ifndef TLA_EDITION_EN
	.ifndef TLA_EDITION_DE
	.ifndef TLA_EDITION_ES
	.ifndef TLA_EDITION_FR
	.ifndef TLA_EDITION_IT
	.byte 0x30	@ PACKER: left in the buffer by Tileset_Set79TilesB+0x20bb
	.endif
	.endif
	.endif
	.endif
	.endif
