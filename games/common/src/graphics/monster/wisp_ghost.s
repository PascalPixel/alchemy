	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_WispGhostSprites
Monster_WispGhostSprites:
	.incbin "games/common/src/graphics/monster/wisp_ghost.frames.lz"
	.ifeq GAME_TLA
	.byte 0x3e, 0xca	@ PACKER: left in the buffer by Monster_OgreSprites+0xe46
	.endif
