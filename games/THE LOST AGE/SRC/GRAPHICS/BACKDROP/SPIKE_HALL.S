@ Battle_SpikeHallBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_SpikeHallBackdrop
Battle_SpikeHallBackdrop:
	.incbin "GRAPHICS/BACKDROP/SPIKE_HALL.gbapal"
	.incbin "GRAPHICS/BACKDROP/SPIKE_HALL.bitmap.d7"
