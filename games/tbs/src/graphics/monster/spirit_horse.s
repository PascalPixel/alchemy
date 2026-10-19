	.section .rodata
	.global Monster_SpiritHorseSprites
Monster_SpiritHorseSprites:
	.incbin "games/tbs/src/graphics/monster/spirit_horse.frames.lz"
	.byte 0x31, 0x20, 0x83	@ PACKER: left in the buffer by Monster_SeaBeastSprites+0xcc5
