	.section .rodata
	.global Ending_VillagersPictureA
Ending_VillagersPictureA:
	.incbin "games/tla/src/graphics/ending/villagers_a.gbapal"
	.incbin "games/tla/src/graphics/ending/villagers_a.bitmap.d7"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
