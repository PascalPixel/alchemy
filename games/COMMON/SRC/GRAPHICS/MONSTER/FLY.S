	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_FlySprites
Monster_FlySprites:
	.incbin "GRAPHICS/MONSTER/FLY.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_WalkingPlantSprites at +0x773 */
	.byte 0x13
	.endif
