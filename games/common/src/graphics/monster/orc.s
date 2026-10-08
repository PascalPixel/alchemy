	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_OrcSprites
Monster_OrcSprites:
	.incbin "graphics/monster/orc.frames.lz"
	.ifeq GAME_TLA
	.byte 0xfe, 0x7b	@ PACKER: left in the buffer by Monster_GriffinSprites+0x260e
	.endif
