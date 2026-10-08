	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Slots_FramePicture
Slots_FramePicture:
	.incbin "GRAPHICS/SLOTS/FRAME.gbapal"
	.incbin "GRAPHICS/SLOTS/FRAME.4bpp.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_AerialViewBackdrop at +0x781 */
	.byte 0x0f, 0x8b, 0xa4
	.endif
