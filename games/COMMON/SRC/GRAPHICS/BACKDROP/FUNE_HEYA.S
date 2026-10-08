	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_FuneHeyaBackdrop
Battle_FuneHeyaBackdrop:
	.incbin "GRAPHICS/BACKDROP/FUNE_HEYA.gbapal"
	.incbin "GRAPHICS/BACKDROP/FUNE_HEYA.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x39, 0x66	@ PACKER: left in the buffer by Battle_MogoruMoriBackdrop+0x4076
	.endif
