	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_ImiruFuchinBackdrop
Battle_ImiruFuchinBackdrop:
	.incbin "GRAPHICS/BACKDROP/IMIRU_FUCHIN.gbapal"
	.incbin "GRAPHICS/BACKDROP/IMIRU_FUCHIN.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x45, 0x15	@ PACKER: left in the buffer by Battle_FuneHeyaBackdrop+0x383e
	.endif
