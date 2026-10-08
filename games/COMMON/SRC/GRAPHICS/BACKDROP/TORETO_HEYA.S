	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_ToretoHeyaBackdrop
Battle_ToretoHeyaBackdrop:
	.incbin "GRAPHICS/BACKDROP/TORETO_HEYA.gbapal"
	.incbin "GRAPHICS/BACKDROP/TORETO_HEYA.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_KorimaMoriBackdrop at +0x49fa */
	.byte 0x58, 0xe7
	.endif
