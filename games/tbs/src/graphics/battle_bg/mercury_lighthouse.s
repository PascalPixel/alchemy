	.section .rodata
	.global Battle_MercuryLighthouseBackdrop
Battle_MercuryLighthouseBackdrop:
	.incbin "games/tbs/src/graphics/battle_bg/mercury_lighthouse.gbapal"
	.incbin "games/tbs/src/graphics/battle_bg/mercury_lighthouse.bitmap.d7"
	.byte 0x5d, 0xf5	@ PACKER: left in the buffer by Battle_GomaCaveBackdrop+0x4032
