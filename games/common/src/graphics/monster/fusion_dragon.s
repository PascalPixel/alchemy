	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_FusionDragonSprites
Monster_FusionDragonSprites:
	.incbin "games/common/src/graphics/monster/fusion_dragon.frames.lz"
	.ifeq GAME_TLA
	.byte 0x70	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x47f3
	.endif
