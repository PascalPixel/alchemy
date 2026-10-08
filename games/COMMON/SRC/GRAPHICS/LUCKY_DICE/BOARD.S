	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global LuckyDice_Board
LuckyDice_Board:
	.incbin "GRAPHICS/LUCKY_DICE/BOARD.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/BOARD.8bpp.lz"
	.ifeq GAME_TLA
	.byte 0x81, 0x61, 0xa6	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x2e1d
	.endif
