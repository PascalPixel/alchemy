	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_VinasuChojoBackdrop
Battle_VinasuChojoBackdrop:
	.incbin "GRAPHICS/BACKDROP/VINASU_CHOJO.gbapal"
	.incbin "GRAPHICS/BACKDROP/VINASU_CHOJO.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x34, 0x54	@ PACKER: left in the buffer by Battle_VinasuChojoStormBackdrop+0x35ee
	.endif
