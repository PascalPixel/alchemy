	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_WingedDragonSprites
Monster_WingedDragonSprites:
	.incbin "games/common/src/graphics/monster/winged_dragon.frames.lz"
	.ifeq GAME_TLA
	.ifdef TBS_EDITION_JA
	.byte 0xee	@ PACKER: left in the buffer by Ui_Icons+0x4f53
	.else
	.byte 0x8c	@ PACKER: left in the buffer by Ui_Icons+0x4f53
	.endif
	.endif
