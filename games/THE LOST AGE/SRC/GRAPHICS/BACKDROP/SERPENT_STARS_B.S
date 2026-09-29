@ Battle_SerpentStarsBBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_SerpentStarsBBackdrop
Battle_SerpentStarsBBackdrop:
	.incbin "GRAPHICS/BACKDROP/SERPENT_STARS_B.gbapal"
	.incbin "GRAPHICS/BACKDROP/SERPENT_STARS_B.bitmap.d7"
