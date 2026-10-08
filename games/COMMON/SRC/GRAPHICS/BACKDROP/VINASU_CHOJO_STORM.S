	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_VinasuChojoStormBackdrop
Battle_VinasuChojoStormBackdrop:
	.incbin "GRAPHICS/BACKDROP/VINASU_CHOJO_STORM.gbapal"
	.incbin "GRAPHICS/BACKDROP/VINASU_CHOJO_STORM.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_KorosseoBackdrop at +0x3d3a */
	.byte 0x92, 0x11
	.endif
