	.section .rodata
	.global Battle_SunbeamCloudsBackdrop
Battle_SunbeamCloudsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/sunbeam_clouds.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/sunbeam_clouds.bitmap.d7"
	.byte 0x00, 0x00	@ PACKER: left in the buffer by Battle_SerpentStarsBBackdrop+0x2f5a
