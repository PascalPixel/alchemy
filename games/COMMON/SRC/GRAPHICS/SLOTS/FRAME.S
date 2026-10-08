	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Slots_FramePicture
Slots_FramePicture:
	.incbin "GRAPHICS/SLOTS/FRAME.gbapal"
	.incbin "GRAPHICS/SLOTS/FRAME.4bpp.lz"
	.ifeq GAME_TLA
	.byte 0x0f, 0x8b, 0xa4	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x781
	.endif
