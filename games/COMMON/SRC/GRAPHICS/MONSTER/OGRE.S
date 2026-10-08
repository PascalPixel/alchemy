	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_OgreSprites
Monster_OgreSprites:
	.incbin "GRAPHICS/MONSTER/OGRE.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_LizardManSprites at +0x2869 */
	.byte 0x4f, 0x40, 0x6e
	.endif
