	.section .rodata
	.global Battle_TretInteriorsBackdrop
Battle_TretInteriorsBackdrop:
	.incbin "games/common/src/graphics/backdrop/tret_interiors.gbapal"
	.incbin "games/common/src/graphics/backdrop/tret_interiors.bitmap.d7"
	.byte 0x58, 0xe7	@ PACKER: left in the buffer by Battle_KolimaForestBackdrop+0x49fa
