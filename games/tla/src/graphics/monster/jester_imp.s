	.section .rodata
	.global Monster_JesterImpSprites
Monster_JesterImpSprites:
	.incbin "games/tla/src/graphics/monster/jester_imp.pictures.lz"
	.byte 0x00, 0xe4, 0x1a	@ PACKER: left in the buffer by Monster_ClubTrollSprites+0x2249
