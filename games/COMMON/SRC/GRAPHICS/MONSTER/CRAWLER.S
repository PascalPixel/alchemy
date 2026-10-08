	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_CrawlerSprites
Monster_CrawlerSprites:
	.incbin "GRAPHICS/MONSTER/CRAWLER.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_FireDragonSprites at +0x2369 */
	.byte 0xf0, 0x33, 0x17
	.endif
