	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_ArutinYamaBackdrop
Battle_ArutinYamaBackdrop:
	.incbin "graphics/backdrop/arutin_yama.gbapal"
	.incbin "graphics/backdrop/arutin_yama.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x54, 0x14	@ PACKER: left in the buffer by Battle_GomaDouBackdrop+0x3b4a
	.endif
