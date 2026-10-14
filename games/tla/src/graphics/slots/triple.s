	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_TripleBanner
Slots_TripleBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/slots/triple_ja.gbapal"
	.incbin "games/tla/src/graphics/slots/triple_ja.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/triple_de.gbapal"
	.incbin "games/tla/src/graphics/slots/triple_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/triple_es.gbapal"
	.incbin "games/tla/src/graphics/slots/triple_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/triple_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/triple_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/triple_it.gbapal"
	.incbin "games/tla/src/graphics/slots/triple_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/slots/triple.gbapal"
	.incbin "games/tla/src/graphics/slots/triple.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
