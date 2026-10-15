	.section .rodata
	.global Monster_VultureSprites
Monster_VultureSprites:
	.incbin "games/tla/src/graphics/monster/vulture.frames.lz"
	.byte 0x21, 0xe4, 0x01	@ PACKER: left in the buffer by Monster_SeaSerpentSprites+0x2d95
