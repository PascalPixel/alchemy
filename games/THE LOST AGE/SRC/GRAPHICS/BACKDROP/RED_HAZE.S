@ Battle_RedHazeBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_RedHazeBackdrop
Battle_RedHazeBackdrop:
	.incbin "GRAPHICS/BACKDROP/RED_HAZE.gbapal"
	.incbin "GRAPHICS/BACKDROP/RED_HAZE.bitmap.d7"
