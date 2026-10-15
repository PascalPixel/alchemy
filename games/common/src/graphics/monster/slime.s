	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_SlimeSprites
Monster_SlimeSprites:
	.incbin "games/common/src/graphics/monster/slime.frames.lz"
	.ifeq GAME_TLA
	.byte 0x40, 0x2c	@ PACKER: left in the buffer by Monster_KrakenSprites+0x36c6
	.else
	.byte 0x40, 0x2c	@ PACKER: left in the buffer by Monster_KrakenSprites+0x36c6
	.endif
