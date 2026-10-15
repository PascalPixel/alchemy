	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global gCharacter_DescriptorTable
gCharacter_DescriptorTable:
	.ifeq EDITION_INTERNATIONAL
	.include "games/tbs/src/graphics/sprite/spriteja.spriteblocks"
	.else
	.ifdef TBS_EDITION_IT
	.include "games/tbs/src/graphics/sprite/spriteit.spriteblocks"
	.else
	.include "games/tbs/src/graphics/sprite/sprites.spriteblocks"
	.endif
	.endif
