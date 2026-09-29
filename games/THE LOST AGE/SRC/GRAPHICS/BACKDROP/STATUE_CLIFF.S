@ Battle_StatueCliffBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_StatueCliffBackdrop
Battle_StatueCliffBackdrop:
	.incbin "GRAPHICS/BACKDROP/STATUE_CLIFF.gbapal"
	.incbin "GRAPHICS/BACKDROP/STATUE_CLIFF.bitmap.d7"
