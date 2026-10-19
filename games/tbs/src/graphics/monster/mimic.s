	.section .rodata
	.global Monster_MimicSprites
Monster_MimicSprites:
	.incbin "games/tbs/src/graphics/monster/mimic.frames.lz"
	.byte 0xb9	@ PACKER: left in the buffer by Monster_CrawlerSprites+0x199f
