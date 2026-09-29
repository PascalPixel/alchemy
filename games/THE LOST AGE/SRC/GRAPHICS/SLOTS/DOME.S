@ Slots_DomePicture: its 224-colour palette, then 600 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_DomePicture
Slots_DomePicture:
	.incbin "GRAPHICS/SLOTS/DOME.gbapal"
	.incbin "GRAPHICS/SLOTS/DOME.8bpp.lz"
