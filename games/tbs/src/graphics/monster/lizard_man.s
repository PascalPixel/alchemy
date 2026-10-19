	.section .rodata
	.global Monster_LizardManSprites
Monster_LizardManSprites:
	.incbin "games/tbs/src/graphics/monster/lizard_man.frames.lz"
	.byte 0x47, 0x0b, 0x00	@ PACKER: left in the buffer by Monster_OrcLordSprites+0x340d
