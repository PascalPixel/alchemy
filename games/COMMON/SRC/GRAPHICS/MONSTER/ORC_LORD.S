	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_OrcLordSprites
Monster_OrcLordSprites:
	.incbin "GRAPHICS/MONSTER/ORC_LORD.frames.lz"
	.ifeq GAME_TLA
	.byte 0x0b, 0x00	@ PACKER: left in the buffer by Monster_ManticoreSprites+0x364a
	.endif
