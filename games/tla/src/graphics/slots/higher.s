	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_HigherBanner
Slots_HigherBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/slots/higher_ja.gbapal"
	.incbin "games/tla/src/graphics/slots/higher_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/higher_de.gbapal"
	.incbin "games/tla/src/graphics/slots/higher_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/higher_es.gbapal"
	.incbin "games/tla/src/graphics/slots/higher_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/higher_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/higher_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/higher_it.gbapal"
	.incbin "games/tla/src/graphics/slots/higher_it.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/slots/higher.gbapal"
	.incbin "games/tla/src/graphics/slots/higher.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
