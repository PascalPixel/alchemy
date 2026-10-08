	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_OgreSprites
Monster_OgreSprites:
	.incbin "GRAPHICS/MONSTER/OGRE.frames.lz"
	.ifeq GAME_TLA
	.byte 0x4f, 0x40, 0x6e	@ PACKER: left in the buffer by Monster_LizardManSprites+0x2869
	.endif
