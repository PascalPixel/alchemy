@ Battle_OrbAltarBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_OrbAltarBackdrop
Battle_OrbAltarBackdrop:
	.incbin "GRAPHICS/BACKDROP/ORB_ALTAR.gbapal"
	.incbin "GRAPHICS/BACKDROP/ORB_ALTAR.bitmap.d7"
