@ Slots_GreenDigits: its 16-colour palette, then 160 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_GreenDigits
Slots_GreenDigits:
	.incbin "GRAPHICS/SLOTS/GREEN_DIGITS.gbapal"
	.incbin "GRAPHICS/SLOTS/GREEN_DIGITS.4bpp32x32.lz"
