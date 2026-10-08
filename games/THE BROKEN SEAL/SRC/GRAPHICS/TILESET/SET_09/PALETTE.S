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
	.byte 0xd4	@ PACKER: left in the buffer by Field_Map010+0x133
