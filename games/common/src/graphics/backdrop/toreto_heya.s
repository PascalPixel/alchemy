	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_ToretoHeyaBackdrop
Battle_ToretoHeyaBackdrop:
	.incbin "graphics/backdrop/toreto_heya.gbapal"
	.incbin "graphics/backdrop/toreto_heya.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x58, 0xe7	@ PACKER: left in the buffer by Battle_KorimaMoriBackdrop+0x49fa
	.endif
