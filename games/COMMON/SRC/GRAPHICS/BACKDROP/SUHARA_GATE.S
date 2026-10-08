	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_SuharaGateBackdrop
Battle_SuharaGateBackdrop:
	.incbin "GRAPHICS/BACKDROP/SUHARA_GATE.gbapal"
	.incbin "GRAPHICS/BACKDROP/SUHARA_GATE.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x41, 0x83	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x4442
	.endif
