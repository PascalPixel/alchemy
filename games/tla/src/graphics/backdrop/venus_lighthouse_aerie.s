	.section .rodata
	.global Battle_VenusLighthouseAerieBackdrop
Battle_VenusLighthouseAerieBackdrop:
	.incbin "games/tla/src/graphics/backdrop/venus_lighthouse_aerie.gbapal"
	.incbin "games/tla/src/graphics/backdrop/venus_lighthouse_aerie.bitmap.d7"
	.byte 0x34, 0x54	@ PACKER: left in the buffer by Battle_VenusLighthouseAerieStormBackdrop+0x35ee
