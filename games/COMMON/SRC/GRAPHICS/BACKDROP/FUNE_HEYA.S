	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_FuneHeyaBackdrop
Battle_FuneHeyaBackdrop:
	.incbin "GRAPHICS/BACKDROP/FUNE_HEYA.gbapal"
	.incbin "GRAPHICS/BACKDROP/FUNE_HEYA.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_MogoruMoriBackdrop at +0x4076 */
	.byte 0x39, 0x66
	.endif
