	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.include "games/common/src/graphics/slots/frame.s"
	.include "games/common/src/graphics/slots/icons.s"
	.global Slots_Jackpot
Slots_Jackpot:
	.ifeq EDITION_INTERNATIONAL
	.incbin "common/graphics/slots/jackpot_ja.gbapal"
	.incbin "common/graphics/slots/jackpot_ja.4bpp32x32.lz"
	.else
	.incbin "graphics/slots/jackpot.gbapal"
	.incbin "graphics/slots/jackpot.4bpp32x32.lz"
	.endif
	.ifdef TBS_EDITION_JA
	.byte 0xa2	@ PACKER: left in the buffer by Slots_Icons+0x66b
	.else
	.byte 0x39	@ PACKER: left in the buffer by Slots_Icons+0x64b
	.endif
