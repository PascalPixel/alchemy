@ Ending_EmbracePicture: a 512x120 panorama: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Ending_EmbracePicture
Ending_EmbracePicture:
	.incbin "GRAPHICS/ENDING/EMBRACE.gbapal"
	.incbin "GRAPHICS/ENDING/EMBRACE.bitmap.d7"
