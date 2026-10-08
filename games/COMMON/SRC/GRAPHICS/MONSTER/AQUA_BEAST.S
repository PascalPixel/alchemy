	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_AquaBeastSprites
Monster_AquaBeastSprites:
	.incbin "GRAPHICS/MONSTER/AQUA_BEAST.frames.lz"
	.ifeq GAME_TLA
	.byte 0xba, 0x77, 0xc9	@ PACKER: left in the buffer by Monster_SalamanderSprites+0xce9
	.endif
