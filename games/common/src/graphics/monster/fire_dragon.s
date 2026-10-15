	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_FireDragonSprites
Monster_FireDragonSprites:
	.incbin "games/common/src/graphics/monster/fire_dragon.frames.lz"
	.ifeq GAME_TLA
	.byte 0x0f	@ PACKER: left in the buffer by Monster_WingedDragonSprites+0x4bb3
	.else
	.byte 0x0f	@ PACKER: left in the buffer by Monster_WingedDragonSprites+0x4bb3
	.endif
