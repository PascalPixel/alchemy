@ Slots_Screen: its 16-colour palette, then 600 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_Screen
Slots_Screen:
	.incbin "GRAPHICS/SLOTS/SCREEN.gbapal"
	.incbin "GRAPHICS/SLOTS/SCREEN.4bpp.lz"
