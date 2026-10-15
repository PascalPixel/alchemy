	.section .rodata
	.global Monster_SeaDragonSprites
Monster_SeaDragonSprites:
	.incbin "games/tla/src/graphics/monster/sea_dragon.frames.lz"
	.byte 0xc0	@ PACKER: left in the buffer by Monster_RedBeastSprites+0x2c27
