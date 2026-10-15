	.section .rodata
	.global Battle_BlueRuinsBackdrop
Battle_BlueRuinsBackdrop:
	.incbin "games/tla/src/graphics/backdrop/blue_ruins.gbapal"
	.incbin "games/tla/src/graphics/backdrop/blue_ruins.bitmap.d7"
	.byte 0xf4, 0xa1	@ PACKER: left in the buffer by Battle_MossCaveBackdrop+0x3eba
