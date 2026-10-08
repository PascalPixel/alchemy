	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_FrogSprites
Monster_FrogSprites:
	.incbin "GRAPHICS/MONSTER/FROG.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_SmallBirdSprites at +0xc1b */
	.byte 0xb3
	.endif
