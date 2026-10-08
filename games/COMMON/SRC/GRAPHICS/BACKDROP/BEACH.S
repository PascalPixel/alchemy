	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_BeachBackdrop
Battle_BeachBackdrop:
	.incbin "GRAPHICS/BACKDROP/BEACH.gbapal"
	.incbin "GRAPHICS/BACKDROP/BEACH.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x10, 0xb5	@ PACKER: left in the buffer by Battle_ToretoEdaBackdrop+0x397e
	.endif
