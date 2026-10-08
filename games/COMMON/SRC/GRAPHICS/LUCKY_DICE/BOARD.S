	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global LuckyDice_Board
LuckyDice_Board:
	.incbin "GRAPHICS/LUCKY_DICE/BOARD.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/BOARD.8bpp.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_AerialViewBackdrop at +0x2e1d */
	.byte 0x81, 0x61, 0xa6
	.endif
