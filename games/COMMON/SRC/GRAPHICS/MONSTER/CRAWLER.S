	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_CrawlerSprites
Monster_CrawlerSprites:
	.incbin "GRAPHICS/MONSTER/CRAWLER.frames.lz"
	.ifeq GAME_TLA
	.byte 0xf0, 0x33, 0x17	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x2369
	.endif
