	.section .rodata
	.global Ending_CliffDayPicture
Ending_CliffDayPicture:
	.incbin "games/tla/src/graphics/ending/cliff_day.gbapal"
	.incbin "games/tla/src/graphics/ending/cliff_day.bitmap.d7"
	.global Ending_CliffSunsetPicture
Ending_CliffSunsetPicture:
	.incbin "games/tla/src/graphics/ending/cliff_sunset.gbapal"
	.incbin "games/tla/src/graphics/ending/cliff_sunset.bitmap.d7"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Ending_VillagersPictureA
Ending_VillagersPictureA:
	.incbin "games/tla/src/graphics/ending/villagers_a.gbapal"
	.incbin "games/tla/src/graphics/ending/villagers_a.bitmap.d7"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Ending_VillagersPictureB
Ending_VillagersPictureB:
	.incbin "games/tla/src/graphics/ending/villagers_b.gbapal"
	.incbin "games/tla/src/graphics/ending/villagers_b.bitmap.d7"
	.byte 0x01, 0x05	@ PACKER: left in the buffer by Ending_VillagersPictureA+0x921a
	.global Ending_EmbracePicture
Ending_EmbracePicture:
	.incbin "games/tla/src/graphics/ending/embrace.gbapal"
	.incbin "games/tla/src/graphics/ending/embrace.bitmap.d7"
	.global Ending_ReunionPicture
Ending_ReunionPicture:
	.incbin "games/tla/src/graphics/ending/reunion.gbapal"
	.incbin "games/tla/src/graphics/ending/reunion.bitmap.d7"
	.byte 0x37, 0x16	@ PACKER: left in the buffer by Ending_VillagersPictureB+0x7842
	.global Ending_SunsetPlainPicture
Ending_SunsetPlainPicture:
	.incbin "games/tla/src/graphics/ending/sunset_plain.gbapal"
	.incbin "games/tla/src/graphics/ending/sunset_plain.bitmap.d7"
	.byte 0x5e, 0x21	@ PACKER: left in the buffer by Ending_ReunionPicture+0x5e52
