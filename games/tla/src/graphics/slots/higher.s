	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_HigherBanner
Slots_HigherBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/slots/higher_ja.gbapal"
	.incbin "graphics/slots/higher_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/slots/higher_de.gbapal"
	.incbin "graphics/slots/higher_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/slots/higher_es.gbapal"
	.incbin "graphics/slots/higher_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/slots/higher_fr.gbapal"
	.incbin "graphics/slots/higher_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/slots/higher_it.gbapal"
	.incbin "graphics/slots/higher_it.4bpp32x32.lz"
	.else
	.incbin "graphics/slots/higher.gbapal"
	.incbin "graphics/slots/higher.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
