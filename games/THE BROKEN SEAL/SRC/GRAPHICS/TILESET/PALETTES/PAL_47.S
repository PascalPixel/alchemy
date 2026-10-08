	.section .rodata
	.global Tileset_Palette47
Tileset_Palette47:
	.ifdef TBS_EDITION_DE
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_47_DE.gbapal.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_47_DE.gbapal.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_47_DE.gbapal.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_47_DE.gbapal.lz"
	.else
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_47.gbapal.lz"
	.endif
	.endif
	.endif
	.endif
