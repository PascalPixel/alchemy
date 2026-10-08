	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_TrollSprites
Monster_TrollSprites:
	.incbin "GRAPHICS/MONSTER/TROLL.frames.lz"
	.ifeq GAME_TLA
	.byte 0x2b, 0x00	@ PACKER: left in the buffer by Ui_CommandIcons+0x3222
	.endif
