	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_FuneHeyaBackdrop
Battle_FuneHeyaBackdrop:
	.incbin "games/common/src/graphics/backdrop/fune_heya.gbapal"
	.incbin "games/common/src/graphics/backdrop/fune_heya.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x39, 0x66	@ PACKER: left in the buffer by Battle_MogoruMoriBackdrop+0x4076
	.else
	.byte 0x39, 0x66	@ PACKER: left in the buffer by Battle_MogoruMoriBackdrop+0x4076
	.endif
