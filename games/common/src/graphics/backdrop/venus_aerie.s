	.section .rodata
	.global Battle_VinasuChojoBackdrop
Battle_VinasuChojoBackdrop:
	.incbin "games/common/src/graphics/backdrop/venus_aerie.gbapal"
	.incbin "games/common/src/graphics/backdrop/venus_aerie.bitmap.d7"
	.byte 0x34, 0x54	@ PACKER: left in the buffer by Battle_VinasuChojoStormBackdrop+0x35ee
