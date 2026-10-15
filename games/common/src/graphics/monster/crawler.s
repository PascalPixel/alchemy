	.section .rodata
	.global Monster_CrawlerSprites
Monster_CrawlerSprites:
	.incbin "games/common/src/graphics/monster/crawler.frames.lz"
	.byte 0xf0, 0x33, 0x17	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x2369
