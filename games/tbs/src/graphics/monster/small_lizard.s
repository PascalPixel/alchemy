	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_SmallLizardSprites
Monster_SmallLizardSprites:
	.incbin "games/tbs/src/graphics/monster/small_lizard.frames.lz"
	.ifeq GAME_TLA
	.byte 0x70	@ PACKER: left in the buffer by Monster_FairySprites+0xc6f
	.else
	.byte 0x1d	@ PACKER: left in the buffer by Monster_JellySprites+0xc6f
	.endif
