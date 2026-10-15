	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_ImiruFuchinBackdrop
Battle_ImiruFuchinBackdrop:
	.incbin "games/common/src/graphics/backdrop/imiru_fuchin.gbapal"
	.incbin "games/common/src/graphics/backdrop/imiru_fuchin.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x45, 0x15	@ PACKER: left in the buffer by Battle_FuneHeyaBackdrop+0x383e
	.else
	.byte 0x45, 0x15	@ PACKER: left in the buffer by Battle_FuneHeyaBackdrop+0x383e
	.endif
