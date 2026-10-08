	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_AquaBeastSprites
Monster_AquaBeastSprites:
	.incbin "GRAPHICS/MONSTER/AQUA_BEAST.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_SalamanderSprites at +0xce9 */
	.byte 0xba, 0x77, 0xc9
	.endif
