@ Battle_WaterfallGorgeBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_WaterfallGorgeBackdrop
Battle_WaterfallGorgeBackdrop:
	.incbin "GRAPHICS/BACKDROP/WATERFALL_GORGE.gbapal"
	.incbin "GRAPHICS/BACKDROP/WATERFALL_GORGE.bitmap.d7"
