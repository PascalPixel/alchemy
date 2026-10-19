	.section .rodata
	.global Monster_ManedLionSprites
Monster_ManedLionSprites:
	.incbin "games/tla/src/graphics/monster/maned_lion.frames.lz"
	.byte 0x35	@ PACKER: left in the buffer by Monster_SirenSprites+0x26b3
