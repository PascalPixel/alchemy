	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_SoruShindenBackdrop
Battle_SoruShindenBackdrop:
	.incbin "GRAPHICS/BACKDROP/SORU_SHINDEN.gbapal"
	.incbin "GRAPHICS/BACKDROP/SORU_SHINDEN.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_GrasslandBackdrop at +0x42b6 */
	.byte 0x10, 0x15
	.endif
