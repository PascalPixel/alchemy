	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_FireDragonSprites
Monster_FireDragonSprites:
	.incbin "GRAPHICS/MONSTER/FIRE_DRAGON.frames.lz"
	.ifeq GAME_TLA
	.byte 0x0f	@ PACKER: left in the buffer by Monster_WingedDragonSprites+0x4bb3
	.endif
