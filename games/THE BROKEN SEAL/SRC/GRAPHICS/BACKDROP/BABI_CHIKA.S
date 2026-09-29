@ Battle_BabiChikaBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_BabiChikaBackdrop
Battle_BabiChikaBackdrop:
	.incbin "GRAPHICS/BACKDROP/BABI_CHIKA.gbapal"
	.incbin "GRAPHICS/BACKDROP/BABI_CHIKA.bitmap.d7"
