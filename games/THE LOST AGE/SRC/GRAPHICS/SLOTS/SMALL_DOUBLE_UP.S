@ Slots_SmallDoubleUpBanner: its 16-colour palette, then 16 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_SmallDoubleUpBanner
Slots_SmallDoubleUpBanner:
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP.gbapal"
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP.4bpp.lz"
