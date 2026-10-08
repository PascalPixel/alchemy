	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_HaidiaArashiBackdrop
Battle_HaidiaArashiBackdrop:
	.incbin "graphics/backdrop/haidia_arashi.gbapal"
	.incbin "graphics/backdrop/haidia_arashi.bitmap.d7"
	.ifeq GAME_TLA
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.endif
