	.section .rodata
	.global Monster_OgreSprites
Monster_OgreSprites:
	.incbin "games/tla/src/graphics/monster/ogre.pictures.lz"
	.byte 0x76, 0xbf, 0x83	@ PACKER: left in the buffer by Monster_GargoyleSprites+0x2869
