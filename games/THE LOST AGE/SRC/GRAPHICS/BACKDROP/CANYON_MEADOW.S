@ Battle_CanyonMeadowBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_CanyonMeadowBackdrop
Battle_CanyonMeadowBackdrop:
	.incbin "GRAPHICS/BACKDROP/CANYON_MEADOW.gbapal"
	.incbin "GRAPHICS/BACKDROP/CANYON_MEADOW.bitmap.d7"
