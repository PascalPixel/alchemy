	.section .rodata
	.global Tileset_Palette47
Tileset_Palette47:
	.ifdef TBS_EDITION_DE
	.incbin "graphics/tileset/palettes/pal_47_de.gbapal.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "graphics/tileset/palettes/pal_47_de.gbapal.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "graphics/tileset/palettes/pal_47_de.gbapal.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "graphics/tileset/palettes/pal_47_de.gbapal.lz"
	.else
	.incbin "graphics/tileset/palettes/pal_47.gbapal.lz"
	.endif
	.endif
	.endif
	.endif
