	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Resource_Data012
Resource_Data012:
	.ifeq EDITION_INTERNATIONAL
	.include "graphics/sprite/spriteja.sprites"
	.else
	.include "graphics/sprite/sprites.sprites"
	.endif
