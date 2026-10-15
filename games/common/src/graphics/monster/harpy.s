	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_HarpySprites
Monster_HarpySprites:
	.incbin "games/common/src/graphics/monster/harpy.frames.lz"
	.ifeq GAME_TLA
	.byte 0xbb, 0x01, 0x75	@ PACKER: left in the buffer by Ui_CommandIcons+0x2871
	.else
	.byte 0x0b, 0x01, 0x00	@ PACKER: left in the buffer by Monster_SpearWarriorSprites+0x2871
	.endif
