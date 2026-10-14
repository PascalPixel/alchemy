	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_AxeGoblinSprites
Monster_AxeGoblinSprites:
	.incbin "games/common/src/graphics/monster/axe_goblin.frames.lz"
	.ifeq GAME_TLA
	.byte 0xda, 0x01	@ PACKER: left in the buffer by Ui_CommandIcons+0x21b2
	.endif
