	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_ToretoHeyaBackdrop
Battle_ToretoHeyaBackdrop:
	.incbin "GRAPHICS/BACKDROP/TORETO_HEYA.gbapal"
	.incbin "GRAPHICS/BACKDROP/TORETO_HEYA.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x58, 0xe7	@ PACKER: left in the buffer by Battle_KorimaMoriBackdrop+0x49fa
	.endif
