@ Slots_LowerBanner: its 16-colour palette, then 48 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_LowerBanner
Slots_LowerBanner:
	.incbin "GRAPHICS/SLOTS/LOWER.gbapal"
	.incbin "GRAPHICS/SLOTS/LOWER.4bpp.lz"
