	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_SuhallaGateBackdrop
Battle_SuhallaGateBackdrop:
	.incbin "games/tla/src/graphics/backdrop/suhalla_gate.gbapal"
	.incbin "games/tla/src/graphics/backdrop/suhalla_gate.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x41, 0x83	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x4442
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
