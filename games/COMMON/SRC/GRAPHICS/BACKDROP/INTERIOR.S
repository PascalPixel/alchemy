@ Battle_InteriorBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_InteriorBackdrop
Battle_InteriorBackdrop:
	.incbin "GRAPHICS/BACKDROP/INTERIOR.gbapal"
	.incbin "GRAPHICS/BACKDROP/INTERIOR.bitmap.d7"
