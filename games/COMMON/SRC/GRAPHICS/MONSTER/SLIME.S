	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SlimeSprites
Monster_SlimeSprites:
	.incbin "GRAPHICS/MONSTER/SLIME.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by MONSTER/KRAKEN.o at +0x36c6 */
	.byte 0x40, 0x2c
	.endif
