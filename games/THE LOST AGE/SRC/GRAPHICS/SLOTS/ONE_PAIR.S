@ Slots_OnePairBanner: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_OnePairBanner
Slots_OnePairBanner:
	.incbin "GRAPHICS/SLOTS/ONE_PAIR.gbapal"
	.incbin "GRAPHICS/SLOTS/ONE_PAIR.4bpp.lz"
