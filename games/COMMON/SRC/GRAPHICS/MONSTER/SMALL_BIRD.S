	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SmallBirdSprites
Monster_SmallBirdSprites:
	.incbin "GRAPHICS/MONSTER/SMALL_BIRD.frames.lz"
	.ifeq GAME_TLA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
