	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_FlameSkullSprites
Monster_FlameSkullSprites:
	.incbin "games/common/src/graphics/monster/flame_skull.frames.lz"
	.ifeq GAME_TLA
	.byte 0xc1, 0xb3	@ PACKER: left in the buffer by Monster_SkeletonSprites+0xbea
	.else
	.byte 0xc1, 0xb3	@ PACKER: left in the buffer by Monster_SkeletonSprites+0xbea
	.endif
