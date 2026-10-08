	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_AxeGoblinSprites
Monster_AxeGoblinSprites:
	.incbin "GRAPHICS/MONSTER/AXE_GOBLIN.frames.lz"
	.ifeq GAME_TLA
	.byte 0xda, 0x01	@ PACKER: left in the buffer by Ui_CommandIcons+0x21b2
	.endif
