@ Ending_TheEndPicture: a 512x120 panorama: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Ending_TheEndPicture
Ending_TheEndPicture:
	.incbin "GRAPHICS/ENDING/THE_END.gbapal"
	.incbin "GRAPHICS/ENDING/THE_END.bitmap.d7"
