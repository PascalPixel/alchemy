	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_FlySprites
Monster_FlySprites:
	.incbin "GRAPHICS/MONSTER/FLY.frames.lz"
	.ifeq GAME_TLA
	.byte 0x13	@ PACKER: left in the buffer by Monster_WalkingPlantSprites+0x773
	.endif
