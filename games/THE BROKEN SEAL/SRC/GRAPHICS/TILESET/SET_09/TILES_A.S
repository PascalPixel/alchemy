@ Tileset_Set09TilesA: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set09TilesA
Tileset_Set09TilesA:
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/TILESET/SET_09/TILES_A_ES.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/TILESET/SET_09/TILES_A_ES.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/TILESET/SET_09/TILES_A_ES.4bpp.mtf"
	.else
	.incbin "GRAPHICS/TILESET/SET_09/TILES_A.4bpp.mtf"
	.endif
	.endif
	.endif
