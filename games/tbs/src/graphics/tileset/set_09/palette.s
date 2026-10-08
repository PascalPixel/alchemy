	.section .rodata
	.global Tileset_Set09Palette
Tileset_Set09Palette:
	.ifdef TBS_EDITION_ES
	.incbin "graphics/tileset/set_09/palette_es.gbapal.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "graphics/tileset/set_09/palette_es.gbapal.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "graphics/tileset/set_09/palette_es.gbapal.lz"
	.else
	.incbin "graphics/tileset/set_09/palette.gbapal.lz"
	.endif
	.endif
	.endif
	.byte 0xd4	@ PACKER: left in the buffer by Field_Map010+0x133
