	.section .rodata
	.global Monster_OgreSprites
Monster_OgreSprites:
	.incbin "games/tbs/src/graphics/monster/ogre.frames.lz"
	.byte 0x4f, 0x40, 0x6e	@ PACKER: left in the buffer by Monster_LizardManSprites+0x2869
