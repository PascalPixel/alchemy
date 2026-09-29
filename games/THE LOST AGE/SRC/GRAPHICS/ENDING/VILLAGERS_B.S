@ Ending_VillagersPictureB: a 512x120 panorama: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Ending_VillagersPictureB
Ending_VillagersPictureB:
	.incbin "GRAPHICS/ENDING/VILLAGERS_B.gbapal"
	.incbin "GRAPHICS/ENDING/VILLAGERS_B.bitmap.d7"
