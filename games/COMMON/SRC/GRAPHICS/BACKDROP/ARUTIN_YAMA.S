@ Battle_ArutinYamaBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_ArutinYamaBackdrop
Battle_ArutinYamaBackdrop:
	.incbin "GRAPHICS/BACKDROP/ARUTIN_YAMA.gbapal"
	.incbin "GRAPHICS/BACKDROP/ARUTIN_YAMA.bitmap.d7"
