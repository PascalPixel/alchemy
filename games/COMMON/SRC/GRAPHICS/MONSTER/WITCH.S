	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_WitchSprites
Monster_WitchSprites:
	.incbin "GRAPHICS/MONSTER/WITCH.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_TrollSprites at +0x1702 */
	.byte 0xd8, 0x61
	.endif
