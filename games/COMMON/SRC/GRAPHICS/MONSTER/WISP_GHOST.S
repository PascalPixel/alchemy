	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_WispGhostSprites
Monster_WispGhostSprites:
	.incbin "GRAPHICS/MONSTER/WISP_GHOST.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_OgreSprites at +0xe46 */
	.byte 0x3e, 0xca
	.endif
