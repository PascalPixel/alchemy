	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Character_DescriptorTable
Character_DescriptorTable:
	.ifeq EDITION_INTERNATIONAL
	.include "graphics/sprite/spriteja.spriteblocks"
	.else
	.ifdef TBS_EDITION_IT
	.include "graphics/sprite/spriteit.spriteblocks"
	.else
	.include "graphics/sprite/sprites.spriteblocks"
	.endif
	.endif
