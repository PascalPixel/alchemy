	.section .rodata
	.global Monster_WerewolfSprites
Monster_WerewolfSprites:
	.incbin "games/tla/src/graphics/monster/werewolf.frames.lz"
	.byte 0xb4, 0x85	@ PACKER: left in the buffer by Monster_WhiteKnightSprites+0x3906
