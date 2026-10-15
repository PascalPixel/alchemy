	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_FrogSprites
Monster_FrogSprites:
	.incbin "games/common/src/graphics/monster/frog.frames.lz"
	.ifeq GAME_TLA
	.byte 0xb3	@ PACKER: left in the buffer by Monster_SmallBirdSprites+0xc1b
	.else
	.byte 0xb3	@ PACKER: left in the buffer by Monster_SmallBirdSprites+0xc1b
	.endif
