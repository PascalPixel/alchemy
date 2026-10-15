	.section .rodata
	.global Monster_BlueCrabSprites
Monster_BlueCrabSprites:
	.incbin "games/tla/src/graphics/monster/blue_crab.frames.lz"
	.byte 0x22, 0x79, 0x3f	@ PACKER: left in the buffer by Monster_SkullMageSprites+0x231d
