@ Slots_PerfectBanner: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_PerfectBanner
Slots_PerfectBanner:
	.incbin "GRAPHICS/SLOTS/PERFECT.gbapal"
	.incbin "GRAPHICS/SLOTS/PERFECT.4bpp.lz"
