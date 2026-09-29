@ Slots_Icons: its 240-colour palette, then 424 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_Icons
Slots_Icons:
	.incbin "GRAPHICS/SLOTS/ICONS.gbapal"
	.incbin "GRAPHICS/SLOTS/ICONS.8bpp.lz"
