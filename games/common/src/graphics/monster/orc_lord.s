	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_OrcLordSprites
Monster_OrcLordSprites:
	.incbin "graphics/monster/orc_lord.frames.lz"
	.ifeq GAME_TLA
	.byte 0x0b, 0x00	@ PACKER: left in the buffer by Monster_ManticoreSprites+0x364a
	.endif
