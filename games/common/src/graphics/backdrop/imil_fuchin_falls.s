	.section .rodata
	.global Battle_ImiruFuchinBackdrop
Battle_ImiruFuchinBackdrop:
	.incbin "games/common/src/graphics/backdrop/imil_fuchin_falls.gbapal"
	.incbin "games/common/src/graphics/backdrop/imil_fuchin_falls.bitmap.d7"
	.byte 0x45, 0x15	@ PACKER: left in the buffer by Battle_FuneHeyaBackdrop+0x383e
