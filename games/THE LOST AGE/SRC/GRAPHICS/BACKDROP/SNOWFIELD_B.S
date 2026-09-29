@ Battle_SnowfieldBBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_SnowfieldBBackdrop
Battle_SnowfieldBBackdrop:
	.incbin "GRAPHICS/BACKDROP/SNOWFIELD_B.gbapal"
	.incbin "GRAPHICS/BACKDROP/SNOWFIELD_B.bitmap.d7"
