	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_VinasuChojoStormBackdrop
Battle_VinasuChojoStormBackdrop:
	.incbin "GRAPHICS/BACKDROP/VINASU_CHOJO_STORM.gbapal"
	.incbin "GRAPHICS/BACKDROP/VINASU_CHOJO_STORM.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x92, 0x11	@ PACKER: left in the buffer by Battle_KorosseoBackdrop+0x3d3a
	.endif
