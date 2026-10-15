	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_FlySprites
Monster_FlySprites:
	.incbin "games/common/src/graphics/monster/fly.frames.lz"
	.ifeq GAME_TLA
	.byte 0x13	@ PACKER: left in the buffer by Monster_WalkingPlantSprites+0x773
	.else
	.byte 0x13	@ PACKER: left in the buffer by Monster_WalkingPlantSprites+0x773
	.endif
