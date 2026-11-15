	.include "games/tla/include/edition.inc"
	.section .rodata
	.global gResource_Data012
gResource_Data012:
	.ifeq EDITION_INTERNATIONAL
	.include "games/tla/src/graphics/sprite/sprites_ja.sprites"
	.else
	.include "games/tla/src/graphics/sprite/sprites.sprites"
	.endif
