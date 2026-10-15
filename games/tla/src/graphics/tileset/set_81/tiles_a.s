	.section .rodata
	.global Tileset_Set81TilesA
Tileset_Set81TilesA:
	.incbin "games/tla/src/graphics/tileset/set_81/tiles_a.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x30	@ PACKER: left in the buffer by Tileset_Set79TilesB+0x20bb
	.endif
