@ Slots_PinkDigits: its 16-colour palette, then 40 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_PinkDigits
Slots_PinkDigits:
	.incbin "GRAPHICS/SLOTS/PINK_DIGITS.gbapal"
	.incbin "GRAPHICS/SLOTS/PINK_DIGITS.4bpp16x16.lz"
