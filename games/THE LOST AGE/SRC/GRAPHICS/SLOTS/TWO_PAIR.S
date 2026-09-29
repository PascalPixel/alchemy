@ Slots_TwoPairBanner: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_TwoPairBanner
Slots_TwoPairBanner:
	.incbin "GRAPHICS/SLOTS/TWO_PAIR.gbapal"
	.incbin "GRAPHICS/SLOTS/TWO_PAIR.4bpp.lz"
