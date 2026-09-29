@ Battle_PurpleLighthouseBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_PurpleLighthouseBackdrop
Battle_PurpleLighthouseBackdrop:
	.incbin "GRAPHICS/BACKDROP/PURPLE_LIGHTHOUSE.gbapal"
	.incbin "GRAPHICS/BACKDROP/PURPLE_LIGHTHOUSE.bitmap.d7"
