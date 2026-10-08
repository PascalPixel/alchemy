	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_MakyuriHeyaBackdrop
Battle_MakyuriHeyaBackdrop:
	.incbin "GRAPHICS/BACKDROP/MAKYURI_HEYA.gbapal"
	.incbin "GRAPHICS/BACKDROP/MAKYURI_HEYA.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_GomaDouBackdrop at +0x4032 */
	.byte 0x5d, 0xf5
	.endif
