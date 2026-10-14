	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_LizardManSprites
Monster_LizardManSprites:
	.incbin "games/common/src/graphics/monster/lizard_man.frames.lz"
	.ifeq GAME_TLA
	.byte 0x47, 0x0b, 0x00	@ PACKER: left in the buffer by Monster_OrcLordSprites+0x340d
	.endif
