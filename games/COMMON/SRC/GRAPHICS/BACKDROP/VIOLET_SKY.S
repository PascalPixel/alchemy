@ Battle_VioletSkyBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_VioletSkyBackdrop
Battle_VioletSkyBackdrop:
	.incbin "GRAPHICS/BACKDROP/VIOLET_SKY.gbapal"
	.incbin "GRAPHICS/BACKDROP/VIOLET_SKY.bitmap.d7"
