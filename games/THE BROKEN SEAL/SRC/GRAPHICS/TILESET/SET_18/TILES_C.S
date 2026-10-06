	.section .rodata
	.global Tileset_Set18TilesC
Tileset_Set18TilesC:
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/TILESET/SET_18/TILES_C_ES.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/TILESET/SET_18/TILES_C_ES.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/TILESET/SET_18/TILES_C_ES.4bpp.mtf"
	.else
	.incbin "GRAPHICS/TILESET/SET_18/TILES_C.4bpp.mtf"
	.endif
	.endif
	.endif
