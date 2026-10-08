	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_HarpySprites
Monster_HarpySprites:
	.incbin "GRAPHICS/MONSTER/HARPY.frames.lz"
	.ifeq GAME_TLA
	.byte 0xbb, 0x01, 0x75	@ PACKER: left in the buffer by Ui_CommandIcons+0x2871
	.endif
