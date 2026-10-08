	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SlimeSprites
Monster_SlimeSprites:
	.incbin "GRAPHICS/MONSTER/SLIME.frames.lz"
	.ifeq GAME_TLA
	.byte 0x40, 0x2c	@ PACKER: left in the buffer by Monster_KrakenSprites+0x36c6
	.endif
