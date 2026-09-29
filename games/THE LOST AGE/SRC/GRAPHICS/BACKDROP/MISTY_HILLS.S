@ Battle_MistyHillsBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_MistyHillsBackdrop
Battle_MistyHillsBackdrop:
	.incbin "GRAPHICS/BACKDROP/MISTY_HILLS.gbapal"
	.incbin "GRAPHICS/BACKDROP/MISTY_HILLS.bitmap.d7"
