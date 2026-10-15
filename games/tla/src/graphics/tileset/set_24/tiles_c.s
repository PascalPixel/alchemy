	.section .rodata
	.global Tileset_Set24TilesC
Tileset_Set24TilesC:
	.incbin "games/tla/src/graphics/tileset/set_24/tiles_c.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x1d, 0x1e	@ PACKER: left in the buffer by Field_Map041+0x1976
	.endif
