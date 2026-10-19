	.section .rodata
	.global Monster_SmallLizardSprites
Monster_SmallLizardSprites:
	.incbin "games/tla/src/graphics/monster/small_lizard.frames.lz"
	.byte 0x1d	@ PACKER: left in the buffer by Monster_JellySprites+0xc6f
