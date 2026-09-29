@ Battle_SunbeamCloudsBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_SunbeamCloudsBackdrop
Battle_SunbeamCloudsBackdrop:
	.incbin "GRAPHICS/BACKDROP/SUNBEAM_CLOUDS.gbapal"
	.incbin "GRAPHICS/BACKDROP/SUNBEAM_CLOUDS.bitmap.d7"
