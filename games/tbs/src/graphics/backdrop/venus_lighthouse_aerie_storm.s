	.section .rodata
	.global Battle_VenusLighthouseAerieStormBackdrop
Battle_VenusLighthouseAerieStormBackdrop:
	.incbin "games/tbs/src/graphics/backdrop/venus_lighthouse_aerie_storm.gbapal"
	.incbin "games/tbs/src/graphics/backdrop/venus_lighthouse_aerie_storm.bitmap.d7"
	.byte 0x92, 0x11	@ PACKER: left in the buffer by Battle_ColossoBackdrop+0x3d3a
