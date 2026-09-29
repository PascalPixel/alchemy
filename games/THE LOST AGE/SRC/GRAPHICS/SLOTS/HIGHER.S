@ Slots_HigherBanner: its 16-colour palette, then 48 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_HigherBanner
Slots_HigherBanner:
	.incbin "GRAPHICS/SLOTS/HIGHER.gbapal"
	.incbin "GRAPHICS/SLOTS/HIGHER.4bpp.lz"
