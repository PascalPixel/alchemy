	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_WispGhostSprites
Monster_WispGhostSprites:
	.incbin "GRAPHICS/MONSTER/WISP_GHOST.frames.lz"
	.ifeq GAME_TLA
	.byte 0x3e, 0xca	@ PACKER: left in the buffer by Monster_OgreSprites+0xe46
	.endif
