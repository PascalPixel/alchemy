	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_ManedLionSprites
Monster_ManedLionSprites:
	.incbin "games/common/src/graphics/monster/maned_lion.frames.lz"
	.ifeq GAME_TLA
	.byte 0x35	@ PACKER: left in the buffer by Monster_SirenSprites+0x26b3
	.else
	.byte 0x35	@ PACKER: left in the buffer by Monster_SirenSprites+0x26b3
	.endif
