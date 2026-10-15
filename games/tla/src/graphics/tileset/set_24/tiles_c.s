	.section .rodata
	.global Tileset_Set24TilesC
Tileset_Set24TilesC:
	.incbin "games/tla/src/graphics/tileset/set_24/tiles_c.4bpp.mtf"
	.ifndef TLA_EDITION_EN
	.ifndef TLA_EDITION_DE
	.ifndef TLA_EDITION_ES
	.ifndef TLA_EDITION_FR
	.ifndef TLA_EDITION_IT
	.byte 0x1d, 0x1e	@ PACKER: left in the buffer by Field_Map041+0x1976
	.endif
	.endif
	.endif
	.endif
	.endif
