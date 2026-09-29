@ Battle_BlizzardBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_BlizzardBackdrop
Battle_BlizzardBackdrop:
	.incbin "GRAPHICS/BACKDROP/BLIZZARD.gbapal"
	.incbin "GRAPHICS/BACKDROP/BLIZZARD.bitmap.d7"
