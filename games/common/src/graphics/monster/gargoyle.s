	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_GargoyleSprites
Monster_GargoyleSprites:
	.incbin "games/common/src/graphics/monster/gargoyle.frames.lz"
	.ifeq GAME_TLA
	.byte 0xb1, 0x5b	@ PACKER: left in the buffer by Monster_OrcLordSprites+0x3092
	.endif
