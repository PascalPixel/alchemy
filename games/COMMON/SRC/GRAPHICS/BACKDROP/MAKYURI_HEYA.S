	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_MakyuriHeyaBackdrop
Battle_MakyuriHeyaBackdrop:
	.incbin "GRAPHICS/BACKDROP/MAKYURI_HEYA.gbapal"
	.incbin "GRAPHICS/BACKDROP/MAKYURI_HEYA.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x5d, 0xf5	@ PACKER: left in the buffer by Battle_GomaDouBackdrop+0x4032
	.endif
