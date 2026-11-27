	.include "games/tla/include/edition.inc"
	.section .rodata
	.global Sprite_AnimationMetadata
Sprite_AnimationMetadata:
	.ifeq EDITION_INTERNATIONAL
	.include "games/tla/src/graphics/sprite/sprites_ja.sprites"
	.else
	.include "games/tla/src/graphics/sprite/sprites.sprites"
	.endif
