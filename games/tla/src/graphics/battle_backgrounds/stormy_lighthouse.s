	.section .rodata
	.global Battle_StormyLighthouseBackdrop
Battle_StormyLighthouseBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/stormy_lighthouse.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/stormy_lighthouse.bitmap.d7"
	.byte 0x4a, 0xb4	@ PACKER: left in the buffer by Battle_PurpleLighthouseBackdrop+0x3bfa
