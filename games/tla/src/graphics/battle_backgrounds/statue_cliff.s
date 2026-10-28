	.section .rodata
	.global Battle_StatueCliffBackdrop
Battle_StatueCliffBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/statue_cliff.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/statue_cliff.bitmap.d7"
	.byte 0x03, 0x51	@ PACKER: left in the buffer by Battle_MossCaveBackdrop+0x44ce
