	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_SuharaGateBackdrop
Battle_SuharaGateBackdrop:
	.incbin "GRAPHICS/BACKDROP/SUHARA_GATE.gbapal"
	.incbin "GRAPHICS/BACKDROP/SUHARA_GATE.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Title_IntroGraphicsC at +0x4442 */
	.byte 0x41, 0x83
	.endif
