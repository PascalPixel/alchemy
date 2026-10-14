	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_JackpotBanner
Slots_JackpotBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/common/src/graphics/slots/jackpot_ja.gbapal"
	.incbin "games/common/src/graphics/slots/jackpot_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/jackpot_es.gbapal"
	.incbin "games/tla/src/graphics/slots/jackpot_es.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/slots/jackpot.gbapal"
	.incbin "games/tla/src/graphics/slots/jackpot.4bpp32x32.lz"
	.endif
	.endif
