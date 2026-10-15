	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_SalamanderSprites
Monster_SalamanderSprites:
	.incbin "games/common/src/graphics/monster/salamander.frames.lz"
	.byte 0x62, 0x4f	@ PACKER: left in the buffer by Monster_WispGhostSprites+0xe16
