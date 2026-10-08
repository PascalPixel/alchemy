	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_WalkingPlantSprites
Monster_WalkingPlantSprites:
	.incbin "GRAPHICS/MONSTER/WALKING_PLANT.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_SmallLizardSprites at +0xc17 */
	.byte 0xd5
	.endif
