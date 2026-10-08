	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_ArutamiraDouBackdrop
Battle_ArutamiraDouBackdrop:
	.incbin "GRAPHICS/BACKDROP/ARUTAMIRA_DOU.gbapal"
	.incbin "GRAPHICS/BACKDROP/ARUTAMIRA_DOU.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_KorosseoBackdrop at +0x40d6 */
	.byte 0x41, 0x40
	.endif
