	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_ArutinYamaBackdrop
Battle_ArutinYamaBackdrop:
	.incbin "games/common/src/graphics/backdrop/arutin_yama.gbapal"
	.incbin "games/common/src/graphics/backdrop/arutin_yama.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x54, 0x14	@ PACKER: left in the buffer by Battle_GomaDouBackdrop+0x3b4a
	.else
	.byte 0x13, 0x24	@ PACKER: left in the buffer by Battle_ToretoHeyaBackdrop+0x3b4a
	.endif
