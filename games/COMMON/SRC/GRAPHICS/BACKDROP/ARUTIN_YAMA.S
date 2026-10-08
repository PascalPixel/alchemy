	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_ArutinYamaBackdrop
Battle_ArutinYamaBackdrop:
	.incbin "GRAPHICS/BACKDROP/ARUTIN_YAMA.gbapal"
	.incbin "GRAPHICS/BACKDROP/ARUTIN_YAMA.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_GomaDouBackdrop at +0x3b4a */
	.byte 0x54, 0x14
	.endif
