	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_AxeGoblinSprites
Monster_AxeGoblinSprites:
	.incbin "GRAPHICS/MONSTER/AXE_GOBLIN.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Ui_CommandIcons at +0x21b2 */
	.byte 0xda, 0x01
	.endif
