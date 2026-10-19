	.section .rodata
	.global Slots_FramePicture
Slots_FramePicture:
	.incbin "games/tbs/src/graphics/slots/frame.gbapal"
	.incbin "games/tbs/src/graphics/slots/frame.4bpp.lz"
	.byte 0x0f, 0x8b, 0xa4	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x781
