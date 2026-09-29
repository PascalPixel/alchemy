@ Ending_ReunionPicture: a 512x120 panorama: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Ending_ReunionPicture
Ending_ReunionPicture:
	.incbin "GRAPHICS/ENDING/REUNION.gbapal"
	.incbin "GRAPHICS/ENDING/REUNION.bitmap.d7"
