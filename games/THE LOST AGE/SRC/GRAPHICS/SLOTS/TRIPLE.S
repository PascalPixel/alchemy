@ Slots_TripleBanner: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_TripleBanner
Slots_TripleBanner:
	.incbin "GRAPHICS/SLOTS/TRIPLE.gbapal"
	.incbin "GRAPHICS/SLOTS/TRIPLE.4bpp.lz"
