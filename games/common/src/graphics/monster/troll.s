	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_TrollSprites
Monster_TrollSprites:
	.incbin "games/common/src/graphics/monster/troll.frames.lz"
	.ifeq GAME_TLA
	.byte 0x2b, 0x00	@ PACKER: left in the buffer by Ui_CommandIcons+0x3222
	.endif
