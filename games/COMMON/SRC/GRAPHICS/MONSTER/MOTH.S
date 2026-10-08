	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_MothSprites
Monster_MothSprites:
	.incbin "GRAPHICS/MONSTER/MOTH.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_SmallBirdSprites at +0x9cd */
	.byte 0x92, 0x11, 0x5e
	.endif
