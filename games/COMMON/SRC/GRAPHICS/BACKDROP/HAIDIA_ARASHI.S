	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_HaidiaArashiBackdrop
Battle_HaidiaArashiBackdrop:
	.incbin "GRAPHICS/BACKDROP/HAIDIA_ARASHI.gbapal"
	.incbin "GRAPHICS/BACKDROP/HAIDIA_ARASHI.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: the packer rounded the stream to 4 with 0xff, not with an earlier stream's bytes */
	.balign 4, 0xff
	.endif
