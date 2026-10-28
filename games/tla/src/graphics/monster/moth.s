	.section .rodata
	.global Monster_MothSprites
Monster_MothSprites:
	.incbin "games/tla/src/graphics/monster/moth.pictures.lz"
	.byte 0x92, 0x11, 0x5e	@ PACKER: left in the buffer by Monster_SmallBirdSprites+0x9cd
