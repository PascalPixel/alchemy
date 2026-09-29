@ Battle_SnowfieldBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_SnowfieldBackdrop
Battle_SnowfieldBackdrop:
	.incbin "GRAPHICS/BACKDROP/SNOWFIELD.gbapal"
	.incbin "GRAPHICS/BACKDROP/SNOWFIELD.bitmap.d7"
