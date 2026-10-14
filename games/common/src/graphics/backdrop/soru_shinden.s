	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_SoruShindenBackdrop
Battle_SoruShindenBackdrop:
	.incbin "games/common/src/graphics/backdrop/soru_shinden.gbapal"
	.incbin "games/common/src/graphics/backdrop/soru_shinden.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x10, 0x15	@ PACKER: left in the buffer by Battle_GrasslandBackdrop+0x42b6
	.endif
