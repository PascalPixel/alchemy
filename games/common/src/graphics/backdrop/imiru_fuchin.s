	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_ImiruFuchinBackdrop
Battle_ImiruFuchinBackdrop:
	.incbin "graphics/backdrop/imiru_fuchin.gbapal"
	.incbin "graphics/backdrop/imiru_fuchin.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x45, 0x15	@ PACKER: left in the buffer by Battle_FuneHeyaBackdrop+0x383e
	.endif
