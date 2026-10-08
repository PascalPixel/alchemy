	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_OrcSprites
Monster_OrcSprites:
	.incbin "GRAPHICS/MONSTER/ORC.frames.lz"
	.ifeq GAME_TLA
	.byte 0xfe, 0x7b	@ PACKER: left in the buffer by Monster_GriffinSprites+0x260e
	.endif
