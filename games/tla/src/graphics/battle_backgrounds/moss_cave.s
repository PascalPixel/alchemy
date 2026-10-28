	.section .rodata
	.global Battle_MossCaveBackdrop
Battle_MossCaveBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/moss_cave.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/moss_cave.bitmap.d7"
	.byte 0x33, 0xd8	@ PACKER: left in the buffer by Battle_LavaCaveBackdrop+0x47c6
