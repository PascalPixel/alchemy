	.section .rodata
	.global Battle_DuskCloudsBackdrop
Battle_DuskCloudsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/dusk_clouds.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/dusk_clouds.bitmap.d7"
	.byte 0xc2, 0x4a	@ PACKER: left in the buffer by Battle_SerpentStarsBBackdrop+0x39aa
