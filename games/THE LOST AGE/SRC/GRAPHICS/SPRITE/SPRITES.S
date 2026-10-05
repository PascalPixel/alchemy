	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Resource_Data012
Resource_Data012:
	.ifeq EDITION_INTERNATIONAL
	.include "GRAPHICS/SPRITE/SPRITEJA.sprites"
	.else
	.include "GRAPHICS/SPRITE/SPRITES.sprites"
	.endif
