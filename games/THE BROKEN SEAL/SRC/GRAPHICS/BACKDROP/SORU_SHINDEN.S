@ Battle_SoruShindenBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_SoruShindenBackdrop
Battle_SoruShindenBackdrop:
	.incbin "GRAPHICS/BACKDROP/SORU_SHINDEN.gbapal"
	.incbin "GRAPHICS/BACKDROP/SORU_SHINDEN.bitmap.d7"
