	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_GargoyleSprites
Monster_GargoyleSprites:
	.incbin "GRAPHICS/MONSTER/GARGOYLE.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_OrcLordSprites at +0x3092 */
	.byte 0xb1, 0x5b
	.endif
