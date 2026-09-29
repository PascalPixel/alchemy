@ Battle_AerialViewBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_AerialViewBackdrop
Battle_AerialViewBackdrop:
	.incbin "GRAPHICS/BACKDROP/AERIAL_VIEW.gbapal"
	.incbin "GRAPHICS/BACKDROP/AERIAL_VIEW.bitmap.d7"
