	.section .rodata
	.global Battle_DuskCloudsBackdrop
Battle_DuskCloudsBackdrop:
	.incbin "games/tbs/src/graphics/backdrop/dusk_clouds.gbapal"
	.incbin "games/tbs/src/graphics/backdrop/dusk_clouds.bitmap.d7"
	.byte 0x33, 0x00	@ PACKER: left in the buffer by Battle_ForestBackdrop+0x39aa
