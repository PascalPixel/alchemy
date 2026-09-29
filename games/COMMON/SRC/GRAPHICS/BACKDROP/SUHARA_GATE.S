@ Battle_SuharaGateBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_SuharaGateBackdrop
Battle_SuharaGateBackdrop:
	.incbin "GRAPHICS/BACKDROP/SUHARA_GATE.gbapal"
	.incbin "GRAPHICS/BACKDROP/SUHARA_GATE.bitmap.d7"
