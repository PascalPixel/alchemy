@ Ending_VillagersPictureA: a 512x120 panorama: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Ending_VillagersPictureA
Ending_VillagersPictureA:
	.incbin "GRAPHICS/ENDING/VILLAGERS_A.gbapal"
	.incbin "GRAPHICS/ENDING/VILLAGERS_A.bitmap.d7"
