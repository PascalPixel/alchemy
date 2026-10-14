	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_BeachBackdrop
Battle_BeachBackdrop:
	.incbin "games/common/src/graphics/backdrop/beach.gbapal"
	.incbin "games/common/src/graphics/backdrop/beach.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x10, 0xb5	@ PACKER: left in the buffer by Battle_ToretoEdaBackdrop+0x397e
	.endif
