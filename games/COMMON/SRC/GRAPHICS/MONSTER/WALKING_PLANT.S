	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_WalkingPlantSprites
Monster_WalkingPlantSprites:
	.incbin "GRAPHICS/MONSTER/WALKING_PLANT.frames.lz"
	.ifeq GAME_TLA
	.byte 0xd5	@ PACKER: left in the buffer by Monster_SmallLizardSprites+0xc17
	.endif
