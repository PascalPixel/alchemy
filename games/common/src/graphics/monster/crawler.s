	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_CrawlerSprites
Monster_CrawlerSprites:
	.incbin "graphics/monster/crawler.frames.lz"
	.ifeq GAME_TLA
	.byte 0xf0, 0x33, 0x17	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x2369
	.endif
