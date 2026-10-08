	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_TrollSprites
Monster_TrollSprites:
	.incbin "GRAPHICS/MONSTER/TROLL.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Ui_CommandIcons at +0x3222 */
	.byte 0x2b, 0x00
	.endif
