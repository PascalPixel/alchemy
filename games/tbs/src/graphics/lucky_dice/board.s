	.section .rodata
	.global LuckyDice_Board
LuckyDice_Board:
	.incbin "games/tbs/src/graphics/lucky_dice/board.gbapal"
	.incbin "games/tbs/src/graphics/lucky_dice/board.8bpp.lz"
	.byte 0x81, 0x61, 0xa6	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x2e1d
