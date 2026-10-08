	.section .rodata
	.global Tileset_Set09Palette
Tileset_Set09Palette:
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/TILESET/SET_09/PALETTE_ES.gbapal.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/TILESET/SET_09/PALETTE_ES.gbapal.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/TILESET/SET_09/PALETTE_ES.gbapal.lz"
	.else
	.incbin "GRAPHICS/TILESET/SET_09/PALETTE.gbapal.lz"
	.endif
	.endif
	.endif
	/* PACKER: left in the packer's buffer by MAP_010/MAP.o at +0x133 */
	.byte 0xd4
