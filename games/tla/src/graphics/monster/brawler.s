	.section .rodata
	.global Monster_BrawlerSprites
Monster_BrawlerSprites:
	.incbin "games/tla/src/graphics/monster/brawler.pictures.lz"
	.byte 0x5a, 0x29, 0x0b	@ PACKER: left in the buffer by Monster_BeastLordSprites+0x1df9
