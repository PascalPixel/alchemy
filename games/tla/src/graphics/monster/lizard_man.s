	.section .rodata
	.global Monster_LizardManSprites
Monster_LizardManSprites:
	.incbin "games/tla/src/graphics/monster/lizard_man.frames.lz"
	.byte 0xda, 0xf1, 0x0d	@ PACKER: left in the buffer by Monster_SeaBeastSprites+0x340d
