	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global gResource_Data012
gResource_Data012:
	.ifeq EDITION_INTERNATIONAL
	.include "games/tla/src/graphics/sprite/spriteja.sprites"
	.else
	.include "games/tla/src/graphics/sprite/sprites.sprites"
	.endif
