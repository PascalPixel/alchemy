	.section .rodata
	.global Monster_WitchSprites
Monster_WitchSprites:
	.incbin "games/tla/src/graphics/monster/witch.frames.lz"
	.byte 0xd8, 0x61	@ PACKER: left in the buffer by Monster_TrollSprites+0x1702
