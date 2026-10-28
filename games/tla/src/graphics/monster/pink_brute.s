	.section .rodata
	.global Monster_PinkBruteSprites
Monster_PinkBruteSprites:
	.incbin "games/tla/src/graphics/monster/pink_brute.pictures.lz"
	.byte 0x28, 0x44	@ PACKER: left in the buffer by Monster_BlueDevilSprites+0x2f4a
