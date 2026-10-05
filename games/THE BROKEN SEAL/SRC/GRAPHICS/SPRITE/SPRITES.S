	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Character_DescriptorTable
Character_DescriptorTable:
	.ifeq EDITION_INTERNATIONAL
	.include "GRAPHICS/SPRITE/SPRITEJA.spriteblocks"
	.else
	.ifdef TBS_EDITION_IT
	.include "GRAPHICS/SPRITE/SPRITEIT.spriteblocks"
	.else
	.include "GRAPHICS/SPRITE/SPRITES.spriteblocks"
	.endif
	.endif
