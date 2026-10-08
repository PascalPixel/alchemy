	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_ArutamiraDouBackdrop
Battle_ArutamiraDouBackdrop:
	.incbin "graphics/backdrop/arutamira_dou.gbapal"
	.incbin "graphics/backdrop/arutamira_dou.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x41, 0x40	@ PACKER: left in the buffer by Battle_KorosseoBackdrop+0x40d6
	.endif
