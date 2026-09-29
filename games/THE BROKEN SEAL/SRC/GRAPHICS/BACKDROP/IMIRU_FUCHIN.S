@ Battle_ImiruFuchinBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_ImiruFuchinBackdrop
Battle_ImiruFuchinBackdrop:
	.incbin "GRAPHICS/BACKDROP/IMIRU_FUCHIN.gbapal"
	.incbin "GRAPHICS/BACKDROP/IMIRU_FUCHIN.bitmap.d7"
