	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.global gSprite_AnimationMetadata
gSprite_AnimationMetadata:
	.ifeq EDITION_INTERNATIONAL
	.include "games/tbs/src/graphics/sprite/spriteja.spriteblocks"
	.else
	.ifdef TBS_EDITION_IT
	.include "games/tbs/src/graphics/sprite/spriteit.spriteblocks"
	.else
	.include "games/tbs/src/graphics/sprite/sprites.spriteblocks"
	.endif
	.endif
