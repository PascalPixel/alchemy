	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_BatSprites
Monster_BatSprites:
	.incbin "games/common/src/graphics/monster/bat.frames.lz"
	.ifeq GAME_TLA
	.byte 0xf2, 0x2e, 0xa3	@ PACKER: left in the buffer by Monster_FairySprites+0x7dd
	.endif
