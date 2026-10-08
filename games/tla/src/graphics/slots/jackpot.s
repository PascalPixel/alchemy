	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_JackpotBanner
Slots_JackpotBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/slots/jackpot_1_ja.gbapal"
	.incbin "graphics/slots/jackpot_ja.parts.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/jackpot_1_es.gbapal"
	.incbin "graphics/slots/jackpot_es.parts.lz"
	.else
	.incbin "graphics/slots/jackpot_1.gbapal"
	.incbin "graphics/slots/jackpot.parts.lz"
	.endif
	.endif
