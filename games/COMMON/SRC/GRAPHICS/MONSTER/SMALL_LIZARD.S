	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SmallLizardSprites
Monster_SmallLizardSprites:
	.incbin "GRAPHICS/MONSTER/SMALL_LIZARD.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_FairySprites at +0xc6f */
	.byte 0x70
	.endif
