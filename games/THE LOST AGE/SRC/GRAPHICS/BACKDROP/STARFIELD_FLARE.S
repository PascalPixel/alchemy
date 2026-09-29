@ Battle_StarfieldFlareBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_StarfieldFlareBackdrop
Battle_StarfieldFlareBackdrop:
	.incbin "GRAPHICS/BACKDROP/STARFIELD_FLARE.gbapal"
	.incbin "GRAPHICS/BACKDROP/STARFIELD_FLARE.bitmap.d7"
