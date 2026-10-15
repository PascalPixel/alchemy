	.section .rodata
	.global Monster_ManEaterSprites
Monster_ManEaterSprites:
	.incbin "games/tla/src/graphics/monster/man_eater.frames.lz"
	.byte 0x93, 0x72, 0x0a	@ PACKER: left in the buffer by Monster_SkullMageSprites+0x209d
