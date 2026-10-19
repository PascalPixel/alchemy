	.section .rodata
	.global Monster_HarpySprites
Monster_HarpySprites:
	.incbin "games/tla/src/graphics/monster/harpy.frames.lz"
	.byte 0x0b, 0x01, 0x00	@ PACKER: left in the buffer by Monster_SpearWarriorSprites+0x2871
