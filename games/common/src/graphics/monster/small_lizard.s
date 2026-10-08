	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_SmallLizardSprites
Monster_SmallLizardSprites:
	.incbin "graphics/monster/small_lizard.frames.lz"
	.ifeq GAME_TLA
	.byte 0x70	@ PACKER: left in the buffer by Monster_FairySprites+0xc6f
	.endif
