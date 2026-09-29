@ Battle_FaceTempleBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_FaceTempleBackdrop
Battle_FaceTempleBackdrop:
	.incbin "GRAPHICS/BACKDROP/FACE_TEMPLE.gbapal"
	.incbin "GRAPHICS/BACKDROP/FACE_TEMPLE.bitmap.d7"
