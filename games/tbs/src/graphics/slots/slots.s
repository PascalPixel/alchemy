	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.include "games/tbs/src/graphics/slots/frame.s"
	.include "games/tbs/src/graphics/slots/icons.s"
	.global LuckyWheels_Jackpot
LuckyWheels_Jackpot:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tbs/src/graphics/slots/jackpot_ja.gbapal"
	.incbin "games/tbs/src/graphics/slots/jackpot_ja.4bpp32x32.lz"
	.else
	.incbin "games/tbs/src/graphics/slots/jackpot.gbapal"
	.incbin "games/tbs/src/graphics/slots/jackpot.4bpp32x32.lz"
	.endif
	.ifdef TBS_EDITION_JA
	.byte 0xa2	@ PACKER: left in the buffer by LuckyWheels_Icons+0x66b
	.else
	.byte 0x39	@ PACKER: left in the buffer by LuckyWheels_Icons+0x64b
	.endif
