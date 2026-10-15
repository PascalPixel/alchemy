	.section .rodata
	.global Monster_BeastLordSprites
Monster_BeastLordSprites:
	.incbin "games/tla/src/graphics/monster/beast_lord.frames.lz"
	.byte 0xef	@ PACKER: left in the buffer by Monster_SeaSerpentSprites+0x49ab
