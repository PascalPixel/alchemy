	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_FramePicture
Slots_FramePicture:
	.incbin "graphics/slots/frame.gbapal"
	.incbin "graphics/slots/frame.4bpp.lz"
	.ifeq GAME_TLA
	.byte 0x0f, 0x8b, 0xa4	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x781
	.endif
