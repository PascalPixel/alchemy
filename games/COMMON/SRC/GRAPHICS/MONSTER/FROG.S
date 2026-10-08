	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_FrogSprites
Monster_FrogSprites:
	.incbin "GRAPHICS/MONSTER/FROG.frames.lz"
	.ifeq GAME_TLA
	.byte 0xb3	@ PACKER: left in the buffer by Monster_SmallBirdSprites+0xc1b
	.endif
