	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_CashOutBanner
Slots_CashOutBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/slots/cash_out_ja.gbapal"
	.incbin "graphics/slots/cash_out_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/slots/cash_out_de.gbapal"
	.incbin "graphics/slots/cash_out_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/cash_out_es.gbapal"
	.incbin "graphics/slots/cash_out_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/slots/cash_out_fr.gbapal"
	.incbin "graphics/slots/cash_out_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/slots/cash_out_it.gbapal"
	.incbin "graphics/slots/cash_out_it.4bpp32x32.lz"
	.else
	.incbin "graphics/slots/cash_out.gbapal"
	.incbin "graphics/slots/cash_out.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
