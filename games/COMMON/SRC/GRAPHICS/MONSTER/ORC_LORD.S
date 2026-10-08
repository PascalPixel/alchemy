	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_OrcLordSprites
Monster_OrcLordSprites:
	.incbin "GRAPHICS/MONSTER/ORC_LORD.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_ManticoreSprites at +0x364a */
	.byte 0x0b, 0x00
	.endif
