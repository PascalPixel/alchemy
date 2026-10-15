	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_MakyuriHeyaBackdrop
Battle_MakyuriHeyaBackdrop:
	.incbin "games/common/src/graphics/backdrop/makyuri_heya.gbapal"
	.incbin "games/common/src/graphics/backdrop/makyuri_heya.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x5d, 0xf5	@ PACKER: left in the buffer by Battle_GomaDouBackdrop+0x4032
	.else
	.byte 0xb8, 0xf6	@ PACKER: left in the buffer by Battle_ToretoHeyaBackdrop+0x4032
	.endif
