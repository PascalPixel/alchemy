	.section .rodata
	.global Monster_SwordsmanSprites
Monster_SwordsmanSprites:
	.incbin "games/tla/src/graphics/monster/swordsman.frames.lz"
	.byte 0x40, 0x96, 0x00	@ PACKER: left in the buffer by Monster_SeaSerpentSprites+0x220d
