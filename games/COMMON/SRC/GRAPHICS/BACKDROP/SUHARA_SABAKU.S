@ Battle_SuharaSabakuBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_SuharaSabakuBackdrop
Battle_SuharaSabakuBackdrop:
	.incbin "GRAPHICS/BACKDROP/SUHARA_SABAKU.gbapal"
	.incbin "GRAPHICS/BACKDROP/SUHARA_SABAKU.bitmap.d7"
