	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_ArutinYamaBackdrop
Battle_ArutinYamaBackdrop:
	.incbin "GRAPHICS/BACKDROP/ARUTIN_YAMA.gbapal"
	.incbin "GRAPHICS/BACKDROP/ARUTIN_YAMA.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x54, 0x14	@ PACKER: left in the buffer by Battle_GomaDouBackdrop+0x3b4a
	.endif
