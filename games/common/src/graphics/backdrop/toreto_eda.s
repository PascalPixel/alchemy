	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_ToretoEdaBackdrop
Battle_ToretoEdaBackdrop:
	.incbin "games/common/src/graphics/backdrop/toreto_eda.gbapal"
	.incbin "games/common/src/graphics/backdrop/toreto_eda.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0xfa, 0x8e	@ PACKER: left in the buffer by Battle_HaidiaArashiBackdrop+0x42b2
	.else
	.byte 0xfa, 0x8e	@ PACKER: left in the buffer by Battle_HaidiaArashiBackdrop+0x42b2
	.endif
