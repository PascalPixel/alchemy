	.section .rodata
	.global Battle_SerpentStarsBackdrop
Battle_SerpentStarsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/serpent_stars.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/serpent_stars.bitmap.d7"
	.byte 0x96, 0xf4	@ PACKER: left in the buffer by Battle_StormyLighthouseBackdrop+0x39ae
