	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_OgreSprites
Monster_OgreSprites:
	.incbin "graphics/monster/ogre.frames.lz"
	.ifeq GAME_TLA
	.byte 0x4f, 0x40, 0x6e	@ PACKER: left in the buffer by Monster_LizardManSprites+0x2869
	.endif
