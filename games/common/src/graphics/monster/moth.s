	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_MothSprites
Monster_MothSprites:
	.incbin "games/common/src/graphics/monster/moth.frames.lz"
	.byte 0x92, 0x11, 0x5e	@ PACKER: left in the buffer by Monster_SmallBirdSprites+0x9cd
