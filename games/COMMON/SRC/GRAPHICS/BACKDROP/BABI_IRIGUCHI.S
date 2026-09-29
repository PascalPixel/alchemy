@ Battle_BabiIriguchiBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_BabiIriguchiBackdrop
Battle_BabiIriguchiBackdrop:
	.incbin "GRAPHICS/BACKDROP/BABI_IRIGUCHI.gbapal"
	.incbin "GRAPHICS/BACKDROP/BABI_IRIGUCHI.bitmap.d7"
