	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_SmallBirdSprites
Monster_SmallBirdSprites:
	.incbin "graphics/monster/small_bird.frames.lz"
	.ifeq GAME_TLA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
