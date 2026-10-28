	.section .rodata
	.global Monster_JellySprites
Monster_JellySprites:
	.incbin "games/tla/src/graphics/monster/jelly.pictures.lz"
	.byte 0x39	@ PACKER: left in the buffer by Monster_BlueWispSprites+0xf2f
