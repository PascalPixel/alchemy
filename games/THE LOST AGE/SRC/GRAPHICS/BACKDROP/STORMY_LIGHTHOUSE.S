@ Battle_StormyLighthouseBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_StormyLighthouseBackdrop
Battle_StormyLighthouseBackdrop:
	.incbin "GRAPHICS/BACKDROP/STORMY_LIGHTHOUSE.gbapal"
	.incbin "GRAPHICS/BACKDROP/STORMY_LIGHTHOUSE.bitmap.d7"
