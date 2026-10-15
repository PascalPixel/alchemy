	.section .rodata
	.global Battle_ToretoHeyaBackdrop
Battle_ToretoHeyaBackdrop:
	.incbin "games/common/src/graphics/backdrop/tret_interiors.gbapal"
	.incbin "games/common/src/graphics/backdrop/tret_interiors.bitmap.d7"
	.byte 0x58, 0xe7	@ PACKER: left in the buffer by Battle_KorimaMoriBackdrop+0x49fa
