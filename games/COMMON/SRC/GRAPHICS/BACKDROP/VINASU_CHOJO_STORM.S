@ Battle_VinasuChojoStormBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_VinasuChojoStormBackdrop
Battle_VinasuChojoStormBackdrop:
	.incbin "GRAPHICS/BACKDROP/VINASU_CHOJO_STORM.gbapal"
	.incbin "GRAPHICS/BACKDROP/VINASU_CHOJO_STORM.bitmap.d7"
