	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_Jackpot
Slots_Jackpot:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/slots/jackpot_1_ja.gbapal"
	.incbin "graphics/slots/jackpot_ja.parts.lz"
	.else
	.incbin "graphics/slots/jackpot_1.gbapal"
	.incbin "graphics/slots/jackpot.parts.lz"
	.endif
	.ifdef TBS_EDITION_JA
	.byte 0xa2	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.byte 0x39	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.endif
