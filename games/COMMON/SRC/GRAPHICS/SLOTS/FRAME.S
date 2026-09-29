@ Slots_FramePicture: its 16-colour palette, then 600 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_FramePicture
Slots_FramePicture:
	.incbin "GRAPHICS/SLOTS/FRAME.gbapal"
	.incbin "GRAPHICS/SLOTS/FRAME.4bpp.lz"
