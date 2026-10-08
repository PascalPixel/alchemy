	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_ManedLionSprites
Monster_ManedLionSprites:
	.incbin "GRAPHICS/MONSTER/MANED_LION.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_SirenSprites at +0x26b3 */
	.byte 0x35
	.endif
