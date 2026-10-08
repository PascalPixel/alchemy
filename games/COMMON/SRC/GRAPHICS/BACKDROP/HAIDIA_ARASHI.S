	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_HaidiaArashiBackdrop
Battle_HaidiaArashiBackdrop:
	.incbin "GRAPHICS/BACKDROP/HAIDIA_ARASHI.gbapal"
	.incbin "GRAPHICS/BACKDROP/HAIDIA_ARASHI.bitmap.d7"
	.ifeq GAME_TLA
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.endif
