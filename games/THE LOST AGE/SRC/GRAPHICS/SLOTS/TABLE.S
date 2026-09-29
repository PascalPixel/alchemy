@ Slots_TablePicture: its 224-colour palette, then 600 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_TablePicture
Slots_TablePicture:
	.incbin "GRAPHICS/SLOTS/TABLE.gbapal"
	.incbin "GRAPHICS/SLOTS/TABLE.8bpp.lz"
