	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_AquaBeastSprites
Monster_AquaBeastSprites:
	.incbin "games/common/src/graphics/monster/aqua_beast.frames.lz"
	.ifeq GAME_TLA
	.byte 0xba, 0x77, 0xc9	@ PACKER: left in the buffer by Monster_SalamanderSprites+0xce9
	.endif
