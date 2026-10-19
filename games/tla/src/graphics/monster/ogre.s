	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_OgreSprites
Monster_OgreSprites:
	.incbin "games/tla/src/graphics/monster/ogre.frames.lz"
	.ifeq GAME_TLA
	.byte 0x4f, 0x40, 0x6e	@ PACKER: left in the buffer by Monster_LizardManSprites+0x2869
	.else
	.byte 0x76, 0xbf, 0x83	@ PACKER: left in the buffer by Monster_GargoyleSprites+0x2869
	.endif
