	.section .rodata
	.global Tileset_Set64TilesA
Tileset_Set64TilesA:
	.incbin "GRAPHICS/TILESET/SET_64/TILES_A.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xd8	@ PACKER: left in the buffer by Field_Map148+0x2617
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xd8	@ PACKER: left in the buffer by Field_Map148+0x2617
	.else
	.byte 0x80	@ PACKER: left in the buffer by Field_Map148+0x2617
	.endif
	.endif
