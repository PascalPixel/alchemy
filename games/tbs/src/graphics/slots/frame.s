	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_FramePicture
Slots_FramePicture:
	.incbin "games/tbs/src/graphics/slots/frame.gbapal"
	.incbin "games/tbs/src/graphics/slots/frame.4bpp.lz"
	.ifeq GAME_TLA
	.byte 0x0f, 0x8b, 0xa4	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x781
	.else
	.ifdef TLA_EDITION_EN
	.byte 0x93, 0x34, 0x90	@ PACKER: left in the buffer by Ending_TheEndPicture+0x781
	.else
	.byte 0x00, 0x00, 0x00	@ PACKER: left in the buffer by Ending_TheEndPicture+0x781
	.endif
	.endif
