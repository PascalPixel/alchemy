	.section .rodata
	.global Ending_VillagersPictureB
Ending_VillagersPictureB:
	.incbin "games/tla/src/graphics/ending/villagers_b.gbapal"
	.incbin "games/tla/src/graphics/ending/villagers_b.bitmap.d7"
	.byte 0x01, 0x05	@ PACKER: left in the buffer by Ending_VillagersPictureA+0x921a
