@ Battle_SnowfieldCBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_SnowfieldCBackdrop
Battle_SnowfieldCBackdrop:
	.incbin "GRAPHICS/BACKDROP/SNOWFIELD_C.gbapal"
	.incbin "GRAPHICS/BACKDROP/SNOWFIELD_C.bitmap.d7"
