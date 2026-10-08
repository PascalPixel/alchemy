	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_FireDragonSprites
Monster_FireDragonSprites:
	.incbin "GRAPHICS/MONSTER/FIRE_DRAGON.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_WingedDragonSprites at +0x4bb3 */
	.byte 0x0f
	.endif
