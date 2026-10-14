	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_MenardiSprites
Monster_MenardiSprites:
	.incbin "games/common/src/graphics/monster/menardi.frames.lz"
	.ifeq GAME_TLA
	.byte 0x52	@ PACKER: left in the buffer by Monster_SaturosSprites+0x3da7
	.endif
