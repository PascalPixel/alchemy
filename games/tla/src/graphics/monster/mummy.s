	.section .rodata
	.global Monster_MummySprites
Monster_MummySprites:
	.incbin "games/tla/src/graphics/monster/mummy.pictures.lz"
	.byte 0x72, 0x78	@ PACKER: left in the buffer by Monster_WerewolfSprites+0x2af2
