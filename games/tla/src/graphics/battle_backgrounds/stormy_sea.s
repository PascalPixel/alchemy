	.section .rodata
	.global Battle_StormySeaBackdrop
Battle_StormySeaBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/stormy_sea.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/stormy_sea.bitmap.d7"
	.byte 0x54, 0x28	@ PACKER: left in the buffer by Battle_MossCaveBackdrop+0x462a
