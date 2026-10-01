	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Tileset_Set46TilesD: 512 tiles of 4-bit colour, drawn with one of its set's palettes, packed by
@ the tile compressor.
	.section .rodata
	.global Tileset_Set46TilesD
Tileset_Set46TilesD:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/TILESET/SET_46/TILES_D_JA.4bpp.mtf"
	.else
	.incbin "GRAPHICS/TILESET/SET_46/TILES_D.4bpp.mtf"
	.endif
