	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SalamanderSprites
Monster_SalamanderSprites:
	.incbin "GRAPHICS/MONSTER/SALAMANDER.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_WispGhostSprites at +0xe16 */
	.byte 0x62, 0x4f
	.endif
