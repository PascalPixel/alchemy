	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Tileset_Set46TilesD
Tileset_Set46TilesD:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/TILESET/SET_46/TILES_D_JA.4bpp.mtf"
	.else
	.incbin "GRAPHICS/TILESET/SET_46/TILES_D.4bpp.mtf"
	.endif
