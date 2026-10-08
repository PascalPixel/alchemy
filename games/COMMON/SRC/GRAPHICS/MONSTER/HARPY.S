	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_HarpySprites
Monster_HarpySprites:
	.incbin "GRAPHICS/MONSTER/HARPY.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Ui_CommandIcons at +0x2871 */
	.byte 0xbb, 0x01, 0x75
	.endif
