	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_ImiruFuchinBackdrop
Battle_ImiruFuchinBackdrop:
	.incbin "GRAPHICS/BACKDROP/IMIRU_FUCHIN.gbapal"
	.incbin "GRAPHICS/BACKDROP/IMIRU_FUCHIN.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_FuneHeyaBackdrop at +0x383e */
	.byte 0x45, 0x15
	.endif
