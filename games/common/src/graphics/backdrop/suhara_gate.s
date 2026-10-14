	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_SuharaGateBackdrop
Battle_SuharaGateBackdrop:
	.incbin "games/common/src/graphics/backdrop/suhara_gate.gbapal"
	.incbin "games/common/src/graphics/backdrop/suhara_gate.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x41, 0x83	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x4442
	.endif
