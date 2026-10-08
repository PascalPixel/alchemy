	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_LizardManSprites
Monster_LizardManSprites:
	.incbin "GRAPHICS/MONSTER/LIZARD_MAN.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_OrcLordSprites at +0x340d */
	.byte 0x47, 0x0b, 0x00
	.endif
