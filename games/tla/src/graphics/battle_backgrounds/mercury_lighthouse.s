	.section .rodata
	.global Battle_MercuryLighthouseBackdrop
Battle_MercuryLighthouseBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/mercury_lighthouse.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/mercury_lighthouse.bitmap.d7"
	.byte 0xb8, 0xf6	@ PACKER: left in the buffer by Battle_TretTreeInteriorsBackdrop+0x4032
