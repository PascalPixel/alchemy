@ Slots_OrangeDigits: its 16-colour palette, then 80 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_OrangeDigits
Slots_OrangeDigits:
	.incbin "GRAPHICS/SLOTS/ORANGE_DIGITS.gbapal"
	.incbin "GRAPHICS/SLOTS/ORANGE_DIGITS.4bpp.lz"
