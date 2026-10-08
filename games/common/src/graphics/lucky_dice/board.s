	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global LuckyDice_Board
LuckyDice_Board:
	.incbin "graphics/lucky_dice/board.gbapal"
	.incbin "graphics/lucky_dice/board.8bpp.lz"
	.ifeq GAME_TLA
	.byte 0x81, 0x61, 0xa6	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x2e1d
	.endif
