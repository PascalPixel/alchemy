	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_MimicSprites
Monster_MimicSprites:
	.incbin "graphics/monster/mimic.frames.lz"
	.ifeq GAME_TLA
	.byte 0xb9	@ PACKER: left in the buffer by Monster_CrawlerSprites+0x199f
	.endif
