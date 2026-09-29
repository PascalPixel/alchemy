@ Battle_RamakanSabakuBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_RamakanSabakuBackdrop
Battle_RamakanSabakuBackdrop:
	.incbin "GRAPHICS/BACKDROP/RAMAKAN_SABAKU.gbapal"
	.incbin "GRAPHICS/BACKDROP/RAMAKAN_SABAKU.bitmap.d7"
