	.section .rodata
	.global Battle_PurpleLighthouseBackdrop
Battle_PurpleLighthouseBackdrop:
	.incbin "games/tla/src/graphics/backdrop/purple_lighthouse.gbapal"
	.incbin "games/tla/src/graphics/backdrop/purple_lighthouse.bitmap.d7"
	.byte 0xbe, 0x55	@ PACKER: left in the buffer by Battle_AngelHallBackdrop+0x3cb2
