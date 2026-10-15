	.section .rodata
	.global Battle_MistyHillsBackdrop
Battle_MistyHillsBackdrop:
	.incbin "games/tla/src/graphics/backdrop/misty_hills.gbapal"
	.incbin "games/tla/src/graphics/backdrop/misty_hills.bitmap.d7"
	.byte 0xd7, 0x34	@ PACKER: left in the buffer by Battle_FaceTempleBackdrop+0x3f86
