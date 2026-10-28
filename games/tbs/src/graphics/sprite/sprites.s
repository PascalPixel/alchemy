	.include "games/tbs/include/ed_asm.h"
	.section .rodata
	.global gSprite_AnimationMetadata
gSprite_AnimationMetadata:
	.ifeq EDITION_INTERNATIONAL
	.include "games/tbs/src/graphics/sprite/sprites_ja.spriteblocks"
	.else
	.ifdef TBS_EDITION_IT
	.include "games/tbs/src/graphics/sprite/sprites_it.spriteblocks"
	.else
	.include "games/tbs/src/graphics/sprite/sprites.spriteblocks"
	.endif
	.endif
