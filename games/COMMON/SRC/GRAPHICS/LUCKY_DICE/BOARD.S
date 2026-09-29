@ LuckyDice_Board: its 224-colour palette, then 600 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global LuckyDice_Board
LuckyDice_Board:
	.incbin "GRAPHICS/LUCKY_DICE/BOARD.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/BOARD.8bpp.lz"
