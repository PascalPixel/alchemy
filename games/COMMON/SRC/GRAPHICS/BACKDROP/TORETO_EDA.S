	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_ToretoEdaBackdrop
Battle_ToretoEdaBackdrop:
	.incbin "GRAPHICS/BACKDROP/TORETO_EDA.gbapal"
	.incbin "GRAPHICS/BACKDROP/TORETO_EDA.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0xfa, 0x8e	@ PACKER: left in the buffer by Battle_HaidiaArashiBackdrop+0x42b2
	.endif
