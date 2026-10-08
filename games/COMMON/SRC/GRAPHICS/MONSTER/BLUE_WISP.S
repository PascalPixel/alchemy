	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_BlueWispSprites
Monster_BlueWispSprites:
	.incbin "GRAPHICS/MONSTER/BLUE_WISP.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_SirenSprites at +0x105a */
	.byte 0xe9, 0x10
	.endif
