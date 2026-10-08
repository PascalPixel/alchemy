	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_OrcSprites
Monster_OrcSprites:
	.incbin "GRAPHICS/MONSTER/ORC.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_GriffinSprites at +0x260e */
	.byte 0xfe, 0x7b
	.endif
