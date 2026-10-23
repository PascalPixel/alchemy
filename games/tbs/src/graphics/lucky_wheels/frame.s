	.section .rodata
	.global LuckyWheels_FramePicture
LuckyWheels_FramePicture:
	.incbin "games/tbs/src/graphics/lucky_wheels/frame.gbapal"
	.incbin "games/tbs/src/graphics/lucky_wheels/frame.4bpp.lz"
	.byte 0x0f, 0x8b, 0xa4	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x781
