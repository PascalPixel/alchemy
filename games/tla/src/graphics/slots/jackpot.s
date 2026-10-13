	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_JackpotBanner
Slots_JackpotBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/slots/jackpot_ja.gbapal"
	.incbin "graphics/slots/jackpot_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/jackpot_es.gbapal"
	.incbin "graphics/slots/jackpot_es.4bpp32x32.lz"
	.else
	.incbin "graphics/slots/jackpot.gbapal"
	.incbin "graphics/slots/jackpot.4bpp32x32.lz"
	.endif
	.endif
