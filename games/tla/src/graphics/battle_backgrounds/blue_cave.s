	.section .rodata
	.global Battle_BlueCaveBackdrop
Battle_BlueCaveBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/blue_cave.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/blue_cave.bitmap.d7"
	.byte 0x17, 0x0c	@ PACKER: left in the buffer by Battle_DenseForestBackdrop+0x4666
