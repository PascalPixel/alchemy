@ Battle_KorimaMoriBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_KorimaMoriBackdrop
Battle_KorimaMoriBackdrop:
	.incbin "GRAPHICS/BACKDROP/KORIMA_MORI.gbapal"
	.incbin "GRAPHICS/BACKDROP/KORIMA_MORI.bitmap.d7"
