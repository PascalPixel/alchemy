	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_BeachBackdrop
Battle_BeachBackdrop:
	.incbin "GRAPHICS/BACKDROP/BEACH.gbapal"
	.incbin "GRAPHICS/BACKDROP/BEACH.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_ToretoEdaBackdrop at +0x397e */
	.byte 0x10, 0xb5
	.endif
