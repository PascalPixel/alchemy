	.section .rodata
	.global Battle_ValeStormBackdrop
Battle_ValeStormBackdrop:
	.incbin "games/tbs/src/graphics/backdrop/vale_storm.gbapal"
	.incbin "games/tbs/src/graphics/backdrop/vale_storm.bitmap.d7"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
