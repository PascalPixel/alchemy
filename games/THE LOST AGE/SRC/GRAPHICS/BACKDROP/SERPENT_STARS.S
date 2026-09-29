@ Battle_SerpentStarsBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_SerpentStarsBackdrop
Battle_SerpentStarsBackdrop:
	.incbin "GRAPHICS/BACKDROP/SERPENT_STARS.gbapal"
	.incbin "GRAPHICS/BACKDROP/SERPENT_STARS.bitmap.d7"
