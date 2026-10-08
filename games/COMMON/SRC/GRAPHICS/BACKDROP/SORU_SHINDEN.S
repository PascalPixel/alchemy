	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_SoruShindenBackdrop
Battle_SoruShindenBackdrop:
	.incbin "GRAPHICS/BACKDROP/SORU_SHINDEN.gbapal"
	.incbin "GRAPHICS/BACKDROP/SORU_SHINDEN.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x10, 0x15	@ PACKER: left in the buffer by Battle_GrasslandBackdrop+0x42b6
	.endif
