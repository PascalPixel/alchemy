	.include "games/tla/include/game/ed_asm.h"
	.section .rodata
	.global Slots_FramePicture
Slots_FramePicture:
	.incbin "games/tla/src/graphics/slots/frame.gbapal"
	.incbin "games/tla/src/graphics/slots/frame.4bpp.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x93, 0x34, 0x90	@ PACKER: left in the buffer by Ending_TheEndPicture+0x781
	.else
	.byte 0x00, 0x00, 0x00	@ PACKER: left in the buffer by Ending_TheEndPicture+0x781
	.endif
